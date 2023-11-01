#include <zmq.hpp>

#include "LatencyHistogram.h"

#include "LatencyHistogramEditor.h"


LatencyHistogram::LatencyHistogram()
    : GenericProcessor("Latency Histogram"),
      context(1),  // 1 is the number of I/O threads in the context
      socket(context, ZMQ_PUB)  // ZMQ_PUB means this socket will be publishing messages
{
    socket.bind("tcp://*:5555");  // Replace with your specific address and port
    
    addIntParameter(Parameter::STREAM_SCOPE, "ttl_a", "The first TTL line to measure", 1, 1, 16);
    addIntParameter(Parameter::STREAM_SCOPE, "ttl_b", "The second TTL line to measure", 2, 1, 16);
}



LatencyHistogram::~LatencyHistogram()
{
    socket.close();
    context.close();
}



AudioProcessorEditor* LatencyHistogram::createEditor()
{
    editor = std::make_unique<LatencyHistogramEditor>(this);
    return editor.get();
}


void LatencyHistogram::updateSettings()
{
    settings.update(getDataStreams()); // resets settings to default values
}


bool LatencyHistogram::startAcquisition()
{
	for (auto stream : getDataStreams())
	{
        settings[stream->getStreamId()]->reset();
	}

    return true;
}


void LatencyHistogram::process(AudioBuffer<float>& buffer)
{
    // Get the current timestamp
    uint64 timestamp = getTimestamp(0); // Assuming channel 0, adjust as necessary
    
    // Convert the timestamp to a string or other appropriate format
    std::string timestampStr = std::to_string(timestamp);
    
    // Send the timestamp via ZMQ
    zmq::message_t message(timestampStr.size());
    memcpy(message.data(), timestampStr.data(), timestampStr.size());
    socket.send(message);

    // Check for events
    checkForEvents(true);
}


void LatencyHistogram::handleTTLEvent(TTLEventPtr event)
{
    const bool state = event->getState();

    if (!state)
		return; // only handle rising edges
    
    const uint16 streamId = event->getStreamId();
    
    DataStream* stream = getDataStream(streamId);
    
    if (stream != nullptr)
    {

        const int line = event->getLine();
        double timeInMs = event->getSampleNumber() / stream->getSampleRate() * 1000.0f;
        
		const int ttl_a = stream->getParameter("ttl_a")->getValue();
		const int ttl_b = stream->getParameter("ttl_b")->getValue();

		if (line == ttl_a)
		{
			settings[streamId]->lastTimeTTLA = timeInMs;
		}
		else if (line == ttl_b)
		{

            double latencyMs = timeInMs - settings[streamId]->lastTimeTTLA;
            
            if (latencyMs < maxWindowMs)
            {
				LatencyHistogramEditor* editor = static_cast<LatencyHistogramEditor*>(getEditor());
                editor->addLatency(streamId, latencyMs);
            }

            settings[streamId]->reset();
		}
    }

    
}



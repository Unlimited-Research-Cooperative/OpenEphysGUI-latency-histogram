
#include <zmq.hpp>
#ifndef LATENCYHISTOGRAM_H_DEFINED
#define LATENCYHISTOGRAM_H_DEFINED

#include <ProcessorHeaders.h>

/** Holds settings for one stream's latency histogram */

class HistogramSettings
{

public:

	/** Constructor -- sets default values*/
	HistogramSettings() 
	{ 
		reset();
	}

	/** Destructor */
	~HistogramSettings() {}

	/** Resets values*/
	void reset()
	{
		lastTimeTTLA = -1111.0f;
	}

	/** Last time for TTL A */
	double lastTimeTTLA;

};

class LatencyHistogram : public GenericProcessor
{
public:
	/** The class constructor, used to initialize any members. */
	LatencyHistogram();

	/** The class destructor, used to deallocate memory */
	~LatencyHistogram();

	/** If the processor has a custom editor, this method must be defined to instantiate it. */
	AudioProcessorEditor* createEditor() override;

	/** Called when settings need to be updated */
	void updateSettings() override;

	/** Called when acquisition begins */
	bool startAcquisition() override;

	/** Defines the functionality of the processor.
		The process method is called every time a new data buffer is available.
		Visualizer plugins typically use this method to send data to the canvas for display purposes */
	void process(AudioBuffer<float>& buffer) override;

	/** Handles events received by the processor
		Called automatically for each received event whenever checkForEvents() is called from
		the plugin's process() method */
	void handleTTLEvent(TTLEventPtr event) override;

private:
    zmq::context_t context;
    zmq::socket_t socket;


	StreamSettings<HistogramSettings> settings;

	float maxWindowMs = 1000.0f;

};

#endif
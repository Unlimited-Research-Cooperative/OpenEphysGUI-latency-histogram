#include "LatencyHistogramEditor.h"
#include "LatencyHistogram.h"

LatencyHistogramEditor::LatencyHistogramEditor(GenericProcessor* parentNode) 
    : GenericEditor(parentNode)
{
    // Adding UI elements for TTL lines selection
    addComboBoxParameterEditor("ttl_a", 15, 35);
    addComboBoxParameterEditor("ttl_b", 15, 75);
    
    // Setting the desired width of the editor window
    desiredWidth = 270;
}

void LatencyHistogramEditor::updateSettings()
{
    // Clearing existing histograms
    histograms.clear();

    // Creating new histograms for each data stream
    for (auto stream : getProcessor()->getDataStreams())
    {
        Histogram* h = new Histogram(stream->getStreamId());
        histograms.add(h);
        h->setBounds(115, 30, 150, 90);
        addAndMakeVisible(h);
    }

    // Updating the visibility of histograms based on the selected stream
    selectedStreamHasChanged();
}

void LatencyHistogramEditor::selectedStreamHasChanged()
{
    // Updating visibility of histograms
    for (auto hist : histograms)
    {
        hist->setVisible(hist->streamId == selectedStream);
    }
}

void LatencyHistogramEditor::addLatency(uint16 streamId, double latencyMs)
{
    // Adding a new latency value to the histogram of the specified stream
    for (auto hist : histograms)
    {
        if (hist->streamId == streamId)
        {
            hist->addLatency(latencyMs);
            return;
        }
    }
    // If histogram for the stream is not found, you might want to handle it (e.g., log a warning)
    DBG("Histogram for stream ID " + String(streamId) + " not found.");
}

void LatencyHistogramEditor::startAcquisition()
{
    // Starting the timer for all histograms
    for (auto hist : histograms)
    {
        hist->startTimer(100);
    }
}

void LatencyHistogramEditor::stopAcquisition()
{
    // Stopping the timer for all histograms
    for (auto hist : histograms)
    {
        hist->stopTimer();
    }
}

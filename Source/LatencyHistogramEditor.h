#ifndef LATENCYHISTOGRAMEDITOR_H_DEFINED
#define LATENCYHISTOGRAMEDITOR_H_DEFINED
#include "LatencyHistogramEditor.h"
#include <EditorHeaders.h>
#include "Histogram.h"

class LatencyHistogramEditor : public GenericEditor
{
public:

	/** Constructor */
	LatencyHistogramEditor(GenericProcessor* parentNode);

	/** Destructor */
	~LatencyHistogramEditor() { }

	/** Updates histograms for new streams */
	void updateSettings();

	/** Changes active histogram when selected stream changes */
	void selectedStreamHasChanged();

	/** Starts animation timer */
	void startAcquisition() override;

	/** Stops animation timer */
	void stopAcquisition() override;
	
	/** Adds a new latency value*/
	void addLatency(uint16 streamId, double latencyMs);

private:

	/** Stores the current histograms */
	OwnedArray<Histogram> histograms;

	/** Generates an assertion if this class leaks */
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LatencyHistogramEditor);
};

#endif // LatencyHistogramEDITOR_H_DEFINED
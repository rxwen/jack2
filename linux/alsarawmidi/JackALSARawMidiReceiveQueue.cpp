#include "JackALSARawMidiReceiveQueue.h"
#include "JackError.h"
#include "JackMidiUtil.h"

using Jack::JackALSARawMidiReceiveQueue;

JackALSARawMidiReceiveQueue::
JackALSARawMidiReceiveQueue(snd_rawmidi_t *rawmidi, size_t buffer_size)
{
    buffer = new jack_midi_data_t[buffer_size];
    this->buffer_size = buffer_size;
    this->rawmidi = rawmidi;
}

JackALSARawMidiReceiveQueue::~JackALSARawMidiReceiveQueue()
{
    delete[] buffer;
}

jack_midi_event_t *
JackALSARawMidiReceiveQueue::DequeueEvent()
{
    ssize_t result = snd_rawmidi_read(rawmidi, buffer, buffer_size);
    if (result > 0) {
        event.buffer = buffer;
        event.size = (size_t) result;
        event.time = GetCurrentFrame();
        return &event;
    }
    if (result && (result != -EWOULDBLOCK)) {
        jack_error("JackALSARawMidiReceiveQueue::DequeueEvent - "
                   "snd_rawmidi_read: %s", snd_strerror(result));
    }
    return 0;
}

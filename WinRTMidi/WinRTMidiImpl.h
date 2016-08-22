#pragma once

#include "WinRTMidi.h"
#include "WinRTMidiPortWatcher.h"
#include <memory>

namespace WinRT
{
    public ref class WinRTMidiInPort sealed
    {
    public:
        WinRTMidiInPort();
        virtual ~WinRTMidiInPort();

        void OpenPort(Platform::String^ id);
        void ClosePort(void);

        void RemoveMidiInCallback() {
            mMessageReceivedCallback = nullptr;
        };

    internal:
        // needs to be internal as MidiInMessageReceivedCallbackType is not a WinRT type
        void SetMidiInCallback(WinRTMidiInCallback callback) {
            mMessageReceivedCallback = callback;
        };

    private:
        void OnMidiInMessageReceived(Windows::Devices::Midi::MidiInPort^ sender, Windows::Devices::Midi::MidiMessageReceivedEventArgs^ args);
        Windows::Devices::Midi::MidiInPort^ mMidiInPort;
        long long mLastMessageTime;
        bool mFirstMessage;
        WinRTMidiInCallback mMessageReceivedCallback;
    };

    public ref class WinRTMidiOutPort sealed
    {
    public:
        WinRTMidiOutPort();
        virtual ~WinRTMidiOutPort();
        
        void OpenPort(Platform::String^ id);
        void ClosePort(void);

    internal:
        void Send(const unsigned char* message, unsigned int nBytes);

    private:
        byte* getIBufferDataPtr(Windows::Storage::Streams::IBuffer^ buffer);

        Windows::Devices::Midi::IMidiOutPort^ mMidiOutPort;
        Windows::Storage::Streams::IBuffer^ mBuffer;
        byte* mBufferData;
    };

    class WinRTMidi
    {
    public:
        WinRTMidi(MidiPortChangedCallback callback);
        WinRTMidiPortWatcher^ GetPortWatcher(WinRTMidiPortType type);
        WinRTMidiPortWatcherPtr GetPortWatcherWrapper(WinRTMidiPortType type);
        Platform::String^ getPortId(WinRTMidiPortType type, unsigned int index);

    private:

        WinRTMidiPortWatcher^ mMidiInPortWatcher;
        WinRTMidiPortWatcher^ mMidiOutPortWatcher;
        std::shared_ptr<MidiPortWatcherWrapper> mMidiInPortWatcherWrapper;
        std::shared_ptr<MidiPortWatcherWrapper> mMidiOutPortWatcherWrapper;
    };

    class MidiInPortWrapper
    {
    public:
        MidiInPortWrapper(WinRTMidiInPort^ port, WinRTMidiInCallback callback)
            : mPort(port)
        {
            mPort->SetMidiInCallback(callback);
        }

        WinRTMidiInPort^ getPort() { return mPort; };

    private:
        WinRTMidiInPort^ mPort;
    };

    class MidiOutPortWrapper
    {
    public:
        MidiOutPortWrapper(WinRTMidiOutPort^ port)
            : mPort(port)
        {}

        WinRTMidiOutPort^ getPort() { return mPort; };

    private:
        WinRTMidiOutPort^ mPort;
    };
};


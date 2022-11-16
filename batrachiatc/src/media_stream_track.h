#pragma once

#include "media/base/adapted_video_track_source.h"
#include "platform.h"

typedef struct
{
    int width;
    int height;

    uint8_t* data_y;
    int stride_y;
    uint8_t* data_u;
    int stride_u;
    uint8_t* data_v;
    int stride_v;
} I420Frame;

class IVideoSourceTrack
    : public rtc::AdaptedVideoTrackSource
{
public:
    IVideoSourceTrack(std::string id);
    static IVideoSourceTrack* Create(std::string id);
    void AddFrame(I420Frame* frame);
    bool remote() const;
    bool is_screencast() const;
    webrtc::MediaSourceInterface::SourceState state() const;
    absl::optional<bool> needs_denoising() const;
    
    std::string id;
};

typedef struct
{
    IVideoSourceTrack* track;
} MediaStreamVideoTrack;

/*
MediaStreamTrack

The MediaStreamTrack interface represents a single media track within a stream;
typically, these are audio or video tracks, but other track types may exist as
well.
*/
typedef struct {
    /*
    A Boolean whose value of true if the track is enabled,
    that is allowed to render the media source stream;
    or false if it is disabled, that is not rendering the media source stream
    but silence and blackness. If the track has been disconnected, this value
    can be changed but has no more effect.
    */
    bool enabled;
    /*
    Returns a string containing a unique identifier (GUID) for the track;
    it is generated by the browser.
    */
    char* id;
    /*
    Returns a string set to "audio" if the track is an audio track and to
    "video", if it is a video track. It doesn't change if the track is
    disassociated from its source.
    */
    char* kind;
    /*
    Returns a string containing a user agent-assigned label that identifies the
    track source, as in "internal microphone". The string may be left empty and
    is empty as long as no source has been connected. When the track is
    disassociated from its source, the label is not changed.
    */
    char* label;
    /*
    Returns a Boolean value indicating whether the track is unable to provide
    media data due to a technical issue.
    */
    bool muted;
    /*
    Returns an enumerated value giving the status of the track.
    This will be one of the following values:

    "true" which indicates that an input is connected and does its best-effort
    in providing real-time data. In that case, the output of data can be
    switched on or off using the enabled attribute.

    "false" which indicates that the input is not giving any more data and will
    never provide new data.
    */
    bool ready_state;
    /*
    Returns a Boolean with a value of true if the track is sourced by a
    RTCPeerConnection, false otherwise.
    */
    bool remote;

    /* --------------- not standard --------------- */
    uint32_t width;
    uint32_t height;
    uint16_t frame_rate;
    
    MediaStreamVideoTrack* video_track;
} MediaStreamTrack;

extern "C" EXPORT void media_stream_video_track_add_frame(MediaStreamTrack* track, 
    I420Frame* frame);

extern "C" EXPORT MediaStreamTrack* create_media_stream_video_track(
    char* id,
    char* label,
    uint32_t width,
    uint32_t height,
    uint16_t frame_rate);
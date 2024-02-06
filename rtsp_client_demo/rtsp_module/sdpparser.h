#ifndef SDPPARSER_H
#define SDPPARSER_H

#include <QHostAddress>
#include <QString>


struct Sdp
{
    enum class MediaType
    {
        Video,
        Audio,
        Metadata,
        Unknown
    };

    struct RtpMap
    {
        QString codecName;
        int clockRate = 0;
        int channels = 0;
        bool operator==(const RtpMap& rhs)
        {
            return codecName == rhs.codecName
                   && clockRate == rhs.clockRate
                   && channels == rhs.channels;
        }
        QString toString() const;
    };

    struct Fmtp
    {
        QStringList params;

        QString toString() const;
    };

    struct Media
    {
        int serverPort = 0;
        int payloadType = -1; //< RTP payload type number
        MediaType mediaType = MediaType::Unknown;
        QString control;
        bool sendOnly = false;
        uint32_t ssrc = 0;
        RtpMap rtpmap;
        Fmtp fmtp;
        QStringList sdpAttributes;
        QHostAddress connectionAddress;

        QString toString() const;
    };

    /**
     * Parse SDP session description according to RFC-4566: Session Description Protocol
     * https://tools.ietf.org/html/rfc4566
     * Current implementation is not complete and may fail on correct data
     * (though it work fine in all tests? we've done).
     */
    void parse(const QString& sdp);

    QString controlUrl;
    std::vector<Media> media;
    RtpMap preferredMap;

    QString toString() const;
};


#endif // SDPPARSER_H

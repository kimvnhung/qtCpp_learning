#ifndef HTTP_TYPES_H
#define HTTP_TYPES_H

#include <QUrl>
#include <string_view>


namespace StatusCode {

/**
 * enum has name "Value" for unification purpose only. Real enumeration name is StatusCode (name of namespace).
 * Enum is referred to as StatusCode::Value, if assign real name to enum it will be StatusCode::StatusCode.
 * Namespace is used to associate enum with corresponding functions like toString() and fromString().
 * Namespace is required, since not all such functions can be put to global namespace.
 * E.g. this namespace has convenience function toString(int httpStatusCode).
 */
enum Value: int
{
    undefined = 0,
    _continue = 100,
    switchingProtocols = 101,

    ok = 200,
    created = 201,
    noContent = 204,
    partialContent = 206,
    lastSuccessCode = 299,
    multipleChoices = 300,
    movedPermanently = 301,
    movedTemporarily = 302,
    found = 302,
    seeOther = 303,
    notModified = 304,
    useProxy = 305,
    temporaryRedirect = 307,
    permanentRedirect = 308,

    badRequest = 400,
    unauthorized = 401,
    paymentRequired = 402,
    forbidden = 403,
    notFound = 404,
    notAllowed = 405,
    notAcceptable = 406,
    proxyAuthenticationRequired = 407,
    requestTimeOut = 408,
    conflict = 409,
    gone = 410,
    lengthRequired = 411,
    preconditionFailed = 412,
    requestEntityTooLarge = 413,
    requestUriToLarge = 414,
    unsupportedMediaType = 415,
    rangeNotSatisfiable = 416,
    unprocessableEntity = 422,
    unavailableForLegalReasons = 451,

    internalServerError = 500,
    notImplemented = 501,
    badGateway = 502,
    serviceUnavailable = 503,
    gatewayTimeOut = 504,
};

 std::string toString(Value);
 std::string toString(int);
/** Returns true if  statusCode is 2xx */
 bool isSuccessCode(Value statusCode);
/** Returns true if  statusCode is 2xx */
 bool isSuccessCode(int statusCode);

 bool isMessageBodyAllowed(int statusCode);

} // namespace StatusCode

class Method
{
public:
    static constexpr std::string_view connect = "CONNECT";
    static constexpr std::string_view get = "GET";
    static constexpr std::string_view head = "HEAD";
    static constexpr std::string_view post = "POST";
    static constexpr std::string_view put = "PUT";
    static constexpr std::string_view patch = "PATCH";
    static constexpr std::string_view delete_ = "DELETE";
    static constexpr std::string_view options = "OPTIONS";

    Method() = default;
    Method(const Method&) = default;
    Method(Method&&) = default;

    Method(const std::string_view& str);
    Method(const char* str);
    Method(std::string str);

    Method& operator=(const Method&) = default;
    Method& operator=(Method&&) = default;

    Method& operator=(const std::string_view& str);
    Method& operator=(const char* str);
    Method& operator=(std::string str);

    bool operator<(const Method& right) const;
    bool operator<(const std::string_view& right) const;

    bool operator==(const Method& right) const;
    bool operator!=(const Method& right) const;

    inline bool operator==(const char* right) const
    { return std::strcmp(m_value.c_str(), right) == 0; }

    inline bool operator!=(const char* right) const
    { return std::strcmp(m_value.c_str(), right) != 0; }

    inline bool operator==(const std::string& right) const
    { return std::strcmp(m_value.c_str(), right.c_str()) == 0; }

    inline bool operator!=(const std::string& right) const
    { return std::strcmp(m_value.c_str(), right.c_str()) != 0; }

    inline bool operator==(const std::string_view& right) const
    { return m_value == right; }

    inline bool operator!=(const std::string_view& right) const
    { return m_value != right; }

    const std::string& toString() const;

    static bool isKnown(const std::string_view& str);

    static bool isMessageBodyAllowed(const Method& method);

    static bool isMessageBodyAllowedInResponse(
        const Method& method,
        StatusCode::Value statusCode);

private:
    std::string m_value;
};

/**
 * Represents string like HTTP/1.1, RTSP/1.0.
 */
class MimeProtoVersion
{
public:
    std::string protocol;
    std::string version;

    bool parse(const ConstBufferRefType& data);

    /** Appends serialized data to dstBuffer. */
    void serialize(nx::Buffer* const dstBuffer) const;

    bool operator==(const MimeProtoVersion& right) const
    {
        return protocol == right.protocol
               && version == right.version;
    }

    bool operator!=(const MimeProtoVersion& right) const
    {
        return !(*this == right);
    }

    friend bool operator < (const MimeProtoVersion& lhs, const MimeProtoVersion& rhs)
    {
        return lhs.version < rhs.version;
    }
};

static const MimeProtoVersion http_1_0 = { "HTTP", "1.0" };
static const MimeProtoVersion http_1_1 = { "HTTP", "1.1" };

class RequestLine
{
public:
    Method method;

    // FIXME: URL is not correct structure here, should use more general structure.
    //        See: RFC 2616, section-5.1.2.
    QUrl url;
    MimeProtoVersion version;

    bool parse(const ConstBufferRefType& data);
    /** Appends serialized data to dstBuffer. */
    void serialize(nx::Buffer* dstBuffer) const;
    std::string toString() const;

    enum class EncodeUrlParts { all, authority };

    /**
     * Encodes the URL before putting it into the HTTP request.
     */
    std::string encodeUrl(
        const nx::utils::Url& url, EncodeUrlParts parts = EncodeUrlParts::all) const;
};


#endif // HTTP_TYPES_H

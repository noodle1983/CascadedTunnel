#key:
#   SubMessage 
#   Message

##############################################
#MessageList
#    MessageName                TypeValue
##############################################
MessageList
    MsgHeader                   0x00
    HeartbeatReq                0x01
    HeartbeatRsp                0x02
    NewConnection               0x03
    ProxyReq                    0x04
    ProxyRsp                    0x05

##############################################
#MessageList
#    MessageName                TypeValue   
##############################################
SubMessageList
    DateTimeIEI          Timestamp   0x14

##############################################
#SubMessage Name
#    FieldName    FieldType    Optional
##############################################
SubMessage Header
    Length           Length32  M
    MessageType      MsgId32   M
    ProxyFd          Uint32    M


##############################################
#Message Name
#    FieldName    FieldType    InitValue
##############################################
Message MsgHeader
    include Header

Message  HeartbeatReq
    include Header

Message  HeartbeatRsp
    include Header

Message NewConnection 
    include Header

Message ProxyReq
    include Header
    Payload          PlainString      M

Message ProxyRsp
    include Header
    Payload          PlainString      M


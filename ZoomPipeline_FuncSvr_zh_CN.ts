<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>DialogAddressInput</name>
    <message>
        <source>Dialog</source>
        <translation type="obsolete">对话框</translation>
    </message>
    <message>
        <location filename="dialogaddressinput.ui" line="14"/>
        <source>Participate in CLUSTER</source>
        <translation>加入集群</translation>
    </message>
    <message>
        <location filename="dialogaddressinput.ui" line="22"/>
        <source>Addr</source>
        <translation>地址</translation>
    </message>
    <message>
        <location filename="dialogaddressinput.ui" line="36"/>
        <source>Port</source>
        <translation>端口</translation>
    </message>
</context>
<context>
    <name>ExampleServer::st_clientNodeAppLayer</name>
    <message>
        <location filename="smartlink/st_clientnode_app_imp.cpp" line="104"/>
        <source>Database Access Error :</source>
        <translation>数据库访问错误 :</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_app_imp.cpp" line="135"/>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="252"/>
        <source>Broken Message stMsg_UploadUserListReq, size not correct.</source>
        <translation>消息长度不匹配。stMsg_UploadUserListReq.</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="27"/>
        <source>try to get relations Failed! </source>
        <translation>试图获取权属关系失败!</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="60"/>
        <source>try to del old relations Failed! </source>
        <translation>试图删除旧的权属关系失败!</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="81"/>
        <source>try to insert new relations Failed! </source>
        <translation>试图插入新的权属关系失败!</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="113"/>
        <source>To-server Message Failed.</source>
        <translation>发往服务器的消息解译/处理失败.</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="125"/>
        <source>Client To Server Message Failed.</source>
        <translation>客户端发往主机的程序解译失败。</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="132"/>
        <source>warning, UUID 0xFFFFFFFF.ignore</source>
        <translation>警告，UUID为 0xFFFFFFF 的消息被忽略</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="138"/>
        <source>Bad UUID %1. Client Kicked out</source>
        <translation>非法的 UUID %1。立刻踢出套接字</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="153"/>
        <source>Destin ID </source>
        <translation>目的节点ID </translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="153"/>
        <source> is not currently logged in.</source>
        <translation>尚未在本服务器登记。</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="203"/>
        <source>Broken Message stMsg_ClientLoginReq, size not correct.</source>
        <translation>消息长度不匹配。stMsg_ClientLoginReq。</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="210"/>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="290"/>
        <source>Message type not supported.</source>
        <translation>消息类型不被支持。</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="267"/>
        <source>Broken Message stMsg_DownloadUserListReq, size not correct.</source>
        <translation>消息长度不匹配。stMsg_DownloadUserListReq。</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="282"/>
        <source>Broken Message stMsg_ClientLogoutReq, size not correct.</source>
        <translation>消息长度不匹配。stMsg_ClientLogoutReq。</translation>
    </message>
</context>
<context>
    <name>ExampleServer::st_clientNode_baseTrans</name>
    <message>
        <location filename="smartlink/st_clientnode_basetrans.cpp" line="240"/>
        <source>Client Send a unknown start Header %1 %2. Close client immediately.</source>
        <translation>客户发出了一个未知的开始头 %1 %2。立刻关闭连接。</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_basetrans.cpp" line="270"/>
        <location filename="smartlink/st_clientnode_basetrans.cpp" line="283"/>
        <source>Client ID is invalid! Close client immediatly.</source>
        <translation>节点的ID是不合标准的，立刻关闭该节点的套接字。</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_basetrans.cpp" line="290"/>
        <source>Client ID Changed in Runtime! Close client immediatly.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_basetrans.cpp" line="306"/>
        <source>Client </source>
        <translation>节点 </translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_basetrans.cpp" line="306"/>
        <source> is dead, kick out.</source>
        <translation>丢失心跳，终止该节点的连接。</translation>
    </message>
</context>
<context>
    <name>ExampleServer::st_client_table</name>
    <message>
        <location filename="smartlink/st_client_table.cpp" line="314"/>
        <source>Send Initial UUIDs to Remote Svr:</source>
        <translation>打包发送目前所有的 UUID 到远程服务器：</translation>
    </message>
    <message>
        <location filename="smartlink/st_client_table.cpp" line="362"/>
        <source>Recieved %1 bytes Msg from </source>
        <translation>收到了 %1 字节数据，来源</translation>
    </message>
    <message>
        <location filename="smartlink/st_client_table.cpp" line="387"/>
        <source>Recieved remote %1 client uuid(s) from svr </source>
        <translation>收到了远端服务器拥有的 %1 个客户端UUID。远端服务器 </translation>
    </message>
    <message>
        <location filename="smartlink/st_client_table.cpp" line="415"/>
        <source>Removed remote %1 client uuid(s) from svr </source>
        <translation>移除了 %1 个属于远端服务器的 UUID。 远端服务器 </translation>
    </message>
</context>
<context>
    <name>MainDialog</name>
    <message>
        <source>MainDialog</source>
        <translation type="vanished">功能测试客户端</translation>
    </message>
    <message>
        <source>Control</source>
        <translation type="vanished">物理连接控制</translation>
    </message>
    <message>
        <source>IP</source>
        <translation type="vanished">IP</translation>
    </message>
    <message>
        <source>Port</source>
        <translation type="vanished">端口</translation>
    </message>
    <message>
        <source>Connect</source>
        <translation type="vanished">连接</translation>
    </message>
    <message>
        <source>BoxToServer</source>
        <translation type="obsolete">主机和服务器测试</translation>
    </message>
    <message>
        <source>login</source>
        <translation type="obsolete">登入</translation>
    </message>
    <message>
        <source>Box Serial Number</source>
        <translation type="obsolete">主机序列号</translation>
    </message>
    <message>
        <source>Box ID </source>
        <translation type="obsolete">主机ID</translation>
    </message>
    <message>
        <source>Regisit</source>
        <translation type="obsolete">注册</translation>
    </message>
    <message>
        <source>Log in</source>
        <translation type="obsolete">登入</translation>
    </message>
    <message>
        <source>Time Correct</source>
        <translation type="obsolete">时间校对</translation>
    </message>
    <message>
        <source>Correct</source>
        <translation type="obsolete">校时</translation>
    </message>
    <message>
        <source>relations</source>
        <translation type="obsolete">权属关系</translation>
    </message>
    <message>
        <source>Upload Relations ( , splitted)</source>
        <translation type="vanished">上传下属关系(用逗号分隔各个客户端ID)</translation>
    </message>
    <message>
        <source>Upload</source>
        <translation type="vanished">上传</translation>
    </message>
    <message>
        <source>download Relations</source>
        <translation type="vanished">下载权属关系</translation>
    </message>
    <message>
        <source>user_id</source>
        <translation type="vanished">用户ID</translation>
    </message>
    <message>
        <source>Download</source>
        <translation type="vanished">下载</translation>
    </message>
    <message>
        <source>ClientToServer</source>
        <translation type="obsolete">客户端和服务器测试</translation>
    </message>
    <message>
        <source>username</source>
        <translation type="obsolete">用户名</translation>
    </message>
    <message>
        <source>Log out</source>
        <translation type="vanished">注销</translation>
    </message>
    <message>
        <source>Login</source>
        <translation type="vanished">登入</translation>
    </message>
    <message>
        <source>password</source>
        <translation type="vanished">口令</translation>
    </message>
    <message>
        <source>userid</source>
        <translation type="obsolete">用户ID</translation>
    </message>
    <message>
        <source>Download Hosts</source>
        <translation type="obsolete">下载权属关系</translation>
    </message>
    <message>
        <source>Send Msg</source>
        <translation type="vanished">发送测试数据</translation>
    </message>
    <message>
        <source>Msg</source>
        <translation type="vanished">发送数据</translation>
    </message>
    <message>
        <source>Send To UUID:</source>
        <translation type="vanished">发送到UUID:</translation>
    </message>
    <message>
        <source>Send</source>
        <translation type="vanished">发送</translation>
    </message>
    <message>
        <source>Recieved:</source>
        <translation type="vanished">收到的数据:</translation>
    </message>
    <message>
        <source>Logs</source>
        <translation type="vanished">记录</translation>
    </message>
    <message>
        <source>Recieved Heart-beating msg sended %1 sec(s) ago.</source>
        <translation type="vanished">收到了 %1 秒前发出的心跳测试。</translation>
    </message>
    <message>
        <source>Client Send a unknown start Header %1 %2. Close client immediately.</source>
        <translation type="vanished">客户发出了一个未知的开始头 %1 %2。立刻关闭连接。</translation>
    </message>
    <message>
        <source>Res = %1, ID = %2</source>
        <translation type="obsolete">结果 %1 ID %2</translation>
    </message>
    <message>
        <source>Succeed!</source>
        <translation type="vanished">成功！</translation>
    </message>
    <message>
        <source>Log in succeed!</source>
        <translation type="vanished">登入成功！</translation>
    </message>
    <message>
        <source>But you can connect to another idle svr:%1:%2!</source>
        <translation type="vanished">但是，您可以连接到一个更加空闲的服务器 %1:%2!</translation>
    </message>
    <message>
        <source>Upload failed!</source>
        <translation type="vanished">上传失败!</translation>
    </message>
    <message>
        <source>Download failed!</source>
        <translation type="vanished">下载失败!</translation>
    </message>
    <message>
        <source>Failed!</source>
        <translation type="obsolete">失败！</translation>
    </message>
    <message>
        <source>Log in Failed!</source>
        <translation type="obsolete">登入失败！</translation>
    </message>
    <message>
        <source>Res = %1</source>
        <translation type="vanished">结果 %1 </translation>
    </message>
    <message>
        <source>Time Corrected!</source>
        <translation type="obsolete">时间校对成功！</translation>
    </message>
    <message>
        <source>Time Correct Failed!</source>
        <translation type="obsolete">时间校对失败！</translation>
    </message>
    <message>
        <source>Log in failed!</source>
        <translation type="vanished">登入失败！</translation>
    </message>
    <message>
        <source>upload succeed!</source>
        <translation type="vanished">上传成功！</translation>
    </message>
    <message>
        <source>upload in Failed!</source>
        <translation type="obsolete">上传失败！</translation>
    </message>
    <message>
        <source>download succeed!</source>
        <translation type="vanished">下载成功!</translation>
    </message>
    <message>
        <source>download in Failed!</source>
        <translation type="obsolete">下载失败!</translation>
    </message>
    <message>
        <source>log out succeed!</source>
        <translation type="vanished">注销成功!</translation>
    </message>
</context>
<context>
    <name>QTcpClientTest</name>
    <message>
        <source>Open UUID global Syn File</source>
        <translation type="vanished">打开UUID全局同步文件</translation>
    </message>
</context>
<context>
    <name>QTcpClientTestClass</name>
    <message>
        <source>QTcpClientTest</source>
        <translation type="vanished">QTcpClientTest SSL客户端测试</translation>
    </message>
    <message>
        <source>Message</source>
        <translation type="vanished">信息</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="vanished">文件(&amp;F)</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="vanished">一般</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="vanished">设置</translation>
    </message>
    <message>
        <source>IP</source>
        <translation type="vanished">IP</translation>
    </message>
    <message>
        <source>Port</source>
        <translation type="vanished">端口</translation>
    </message>
    <message>
        <source>SSL(Need Certs)</source>
        <translation type="vanished">SSL连接</translation>
    </message>
    <message>
        <source>Global UUID file</source>
        <translation type="vanished">全局UUID同步文件名</translation>
    </message>
    <message>
        <source>...</source>
        <translation type="vanished">...</translation>
    </message>
    <message>
        <source>Simulation Clients Max</source>
        <translation type="vanished">最大连接客户端数</translation>
    </message>
    <message>
        <source>Payload mean size</source>
        <translation type="vanished">发送信息平均载荷长度</translation>
    </message>
    <message>
        <source>&amp;Connect</source>
        <translation type="vanished">连接(&amp;C)</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation type="vanished">退出(&amp;x)</translation>
    </message>
</context>
<context>
    <name>SmartLink::st_clientNode</name>
    <message>
        <source>Client ID is invalid! Close client immediatly.</source>
        <translation type="obsolete">节点的ID是不合标准的，立刻关闭该节点的套接字。</translation>
    </message>
    <message>
        <source>To-server Message is not currently supported.</source>
        <translation type="obsolete">发往服务器的消息尚未被支持。</translation>
    </message>
    <message>
        <source>Broadcast Message is not currently supported.</source>
        <translation type="obsolete">广播消息尚未被支持。</translation>
    </message>
    <message>
        <source>Destin ID </source>
        <translation type="obsolete">目的节点ID </translation>
    </message>
    <message>
        <source> is not currently logged in.</source>
        <translation type="obsolete">尚未在本服务器登记。</translation>
    </message>
    <message>
        <source>Client </source>
        <translation type="obsolete">节点 </translation>
    </message>
    <message>
        <source> is dead, kick out.</source>
        <translation type="obsolete">丢失心跳，终止该节点的连接。</translation>
    </message>
</context>
<context>
    <name>SmartLink::st_clientNodeAppLayer</name>
    <message>
        <source>Database Access Error :</source>
        <translation type="obsolete">数据库访问错误 :</translation>
    </message>
    <message>
        <source>Broken Message stMsg_UploadUserListReq, size not correct.</source>
        <translation type="obsolete">消息长度不匹配。stMsg_UploadUserListReq。</translation>
    </message>
    <message>
        <source>try to save relations before login!</source>
        <translation type="obsolete">试图在登入前进行权属关系保存!</translation>
    </message>
    <message>
        <source>try to get relations Failed! </source>
        <translation type="obsolete">试图获取权属关系失败!</translation>
    </message>
    <message>
        <source>try to del old relations Failed! </source>
        <translation type="obsolete">试图删除旧的权属关系失败!</translation>
    </message>
    <message>
        <source>try to insert new relations Failed! </source>
        <translation type="obsolete">试图插入新的权属关系失败!</translation>
    </message>
    <message>
        <source>To-server Message Failed.</source>
        <translation type="obsolete">发往服务器的消息解译/处理失败。</translation>
    </message>
    <message>
        <source>Box To Server Message Failed.</source>
        <translation type="obsolete">主机发往服务器的消息解译失败。</translation>
    </message>
    <message>
        <source>Client To Server Message Failed.</source>
        <translation type="obsolete">客户端发往主机的程序解译失败。</translation>
    </message>
    <message>
        <source>warning, UUID 0xFFFFFFFF.ignore</source>
        <translation type="obsolete">警告，UUID为 0xFFFFFFF 的消息被忽略</translation>
    </message>
    <message>
        <source>Bad UUID %1. Client Kicked out</source>
        <translation type="obsolete">非法的 UUID %1。立刻踢出套接字</translation>
    </message>
    <message>
        <source>Broadcast Message is not currently supported.</source>
        <translation type="obsolete">广播消息尚未被支持。</translation>
    </message>
    <message>
        <source>Destin ID </source>
        <translation type="obsolete">目的节点ID </translation>
    </message>
    <message>
        <source> is not currently logged in.</source>
        <translation type="obsolete">尚未在本服务器登记。</translation>
    </message>
    <message>
        <source>Application Layer Version too new.</source>
        <translation type="obsolete">应用层协议版本太高。</translation>
    </message>
    <message>
        <source>Broken Message stMsg_HostRegistReq, size not correct.</source>
        <translation type="obsolete">消息长度不匹配。stMsg_HostRegistReq。</translation>
    </message>
    <message>
        <source>Broken Message stMsg_HostLogonReq, size not correct.</source>
        <translation type="obsolete">消息长度不匹配。stMsg_HostLogonReq。</translation>
    </message>
    <message>
        <source>Broken Message stMsg_ClientLoginReq, size not correct.</source>
        <translation type="obsolete">消息长度不匹配。stMsg_ClientLoginReq。</translation>
    </message>
    <message>
        <source>Message type not supported.</source>
        <translation type="obsolete">消息类型不被支持。</translation>
    </message>
    <message>
        <source>Broken Message stMsg_DownloadUserListReq, size not correct.</source>
        <translation type="obsolete">消息长度不匹配。stMsg_DownloadUserListReq。</translation>
    </message>
    <message>
        <source>Broken Message stMsg_ClientLogoutReq, size not correct.</source>
        <translation type="obsolete">消息长度不匹配。stMsg_ClientLogoutReq。</translation>
    </message>
    <message>
        <source>Broken Message stMsg_GetHostListReq, size not correct.</source>
        <translation type="obsolete">消息长度不匹配。stMsg_GetHostListReq。</translation>
    </message>
</context>
<context>
    <name>SmartLink::st_clientNode_baseTrans</name>
    <message>
        <source>Client Send a unknown start Header %1 %2. Close client immediately.</source>
        <translation type="obsolete">客户发出了一个未知的开始头 %1 %2。立刻关闭连接。</translation>
    </message>
    <message>
        <source>Client ID is invalid! Close client immediatly.</source>
        <translation type="obsolete">节点的ID是不合标准的，立刻关闭该节点的套接字。</translation>
    </message>
    <message>
        <source>Client </source>
        <translation type="obsolete">节点 </translation>
    </message>
    <message>
        <source> is dead, kick out.</source>
        <translation type="obsolete">丢失心跳，终止该节点的连接。</translation>
    </message>
</context>
<context>
    <name>SmartLink::st_client_table</name>
    <message>
        <source>Send Initial UUIDs to Remote Svr:</source>
        <translation type="obsolete">打包发送目前所有的UUID到远程服务器：</translation>
    </message>
    <message>
        <source>Recieved remote user-data to uuid:%1,DATA:%2</source>
        <translation type="obsolete">从远端服务器收到了来自UUID:%1 的数据: %2</translation>
    </message>
    <message>
        <source>Recieved %1 bytes Msg from </source>
        <translation type="obsolete">收到了 %1 字节数据，来源</translation>
    </message>
    <message>
        <source>Recieved remote %1 client uuid(s) from svr </source>
        <translation type="obsolete">收到了远端服务器拥有的 %1 个客户端UUID。远端服务器 </translation>
    </message>
    <message>
        <source>Removed remote %1 client uuid(s) from svr </source>
        <translation type="obsolete">移除了 %1 个属于远端服务器的 UUID。远端服务器 </translation>
    </message>
</context>
<context>
    <name>ZPDatabase::DatabaseResource</name>
    <message>
        <location filename="database/databaseresource.cpp" line="57"/>
        <location filename="database/databaseresource.cpp" line="74"/>
        <location filename="database/databaseresource.cpp" line="83"/>
        <location filename="database/databaseresource.cpp" line="145"/>
        <location filename="database/databaseresource.cpp" line="189"/>
        <source> Connection name </source>
        <translation>数据库连接名称  </translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="57"/>
        <location filename="database/databaseresource.cpp" line="189"/>
        <source> does not exist.</source>
        <translation>尚未定义。</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="33"/>
        <location filename="database/databaseresource.cpp" line="181"/>
        <location filename="database/databaseresource.cpp" line="207"/>
        <location filename="database/databaseresource.cpp" line="267"/>
        <source> Connection removed </source>
        <translation>数据库连接已经移除 </translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="33"/>
        <location filename="database/databaseresource.cpp" line="181"/>
        <location filename="database/databaseresource.cpp" line="207"/>
        <location filename="database/databaseresource.cpp" line="267"/>
        <source> .</source>
        <translation>。</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="75"/>
        <location filename="database/databaseresource.cpp" line="146"/>
        <source> Can not be cloned from database %1.</source>
        <translation>无法从主线程数据库 %1 复制数据库连接。</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="76"/>
        <location filename="database/databaseresource.cpp" line="147"/>
        <source> Err String:</source>
        <translation>错误字符串:</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="84"/>
        <source> has been cloned from database %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="108"/>
        <location filename="database/databaseresource.cpp" line="137"/>
        <location filename="database/databaseresource.cpp" line="282"/>
        <location filename="database/databaseresource.cpp" line="287"/>
        <location filename="database/databaseresource.cpp" line="325"/>
        <location filename="database/databaseresource.cpp" line="355"/>
        <location filename="database/databaseresource.cpp" line="361"/>
        <location filename="database/databaseresource.cpp" line="381"/>
        <location filename="database/databaseresource.cpp" line="386"/>
        <source> Connection  </source>
        <translation>数据库连接 </translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="108"/>
        <source> Need re-connect. MSG=</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="282"/>
        <source> Established.</source>
        <translation>已经建立。</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="287"/>
        <location filename="database/databaseresource.cpp" line="361"/>
        <location filename="database/databaseresource.cpp" line="386"/>
        <source> Can&apos;t be opened. MSG=</source>
        <translation>无法建立，错误消息=</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="308"/>
        <location filename="database/databaseresource.cpp" line="341"/>
        <source> Connection </source>
        <translation>数据库连接</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="308"/>
        <source> has not been added.</source>
        <translation>尚未建立。</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="325"/>
        <source> confirm failed. MSG=</source>
        <translation>数据库连接确认失败，出错提示=</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="341"/>
        <source> has not been opened.</source>
        <translation>尚未建立。</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="137"/>
        <location filename="database/databaseresource.cpp" line="355"/>
        <location filename="database/databaseresource.cpp" line="381"/>
        <source> Re-Established.</source>
        <translation>已经恢复正常。</translation>
    </message>
</context>
<context>
    <name>ZPMainFrame</name>
    <message>
        <location filename="zpmainframe.ui" line="14"/>
        <source>ZPMainFrame</source>
        <translation>伸缩流水线服务器框架</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="47"/>
        <source>Summary</source>
        <translation>概况</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="61"/>
        <source>Settings</source>
        <translation>设置</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="81"/>
        <source>Listeners</source>
        <translation>监听器</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="92"/>
        <location filename="zpmainframe.ui" line="394"/>
        <location filename="zpmainframe.cpp" line="149"/>
        <location filename="zpmainframe.cpp" line="157"/>
        <location filename="zpmainframe.cpp" line="176"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="102"/>
        <location filename="zpmainframe.cpp" line="150"/>
        <source>Addr</source>
        <translation>地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="112"/>
        <location filename="zpmainframe.cpp" line="151"/>
        <location filename="zpmainframe.cpp" line="160"/>
        <source>Port</source>
        <translation></translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="122"/>
        <source>SSL Connection</source>
        <translation>使用SSL连接</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="146"/>
        <location filename="zpmainframe.ui" line="532"/>
        <source>Add</source>
        <translation>添加</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="153"/>
        <source>Del</source>
        <translation>删除</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="160"/>
        <location filename="zpmainframe.ui" line="365"/>
        <location filename="zpmainframe.ui" line="546"/>
        <source>Save</source>
        <translation>保存</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="178"/>
        <source>Task Pools</source>
        <translation>任务池</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="221"/>
        <location filename="zpmainframe.ui" line="266"/>
        <location filename="zpmainframe.ui" line="308"/>
        <source>%v</source>
        <translation>%v</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="273"/>
        <source>Working Task Threads</source>
        <translation>逻辑处理线程数目</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="228"/>
        <source>SSL connection Trans Threads</source>
        <translation>SSL 连接传输线程数目</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="186"/>
        <source>Plain connection Trans Threads</source>
        <translation>普通TCP连接传输线程数目</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="319"/>
        <source>Rabbish Can Items Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="383"/>
        <location filename="zpmainframe.ui" line="989"/>
        <location filename="zpmainframe.cpp" line="161"/>
        <source>Database</source>
        <translation>数据库资源</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="404"/>
        <location filename="zpmainframe.cpp" line="158"/>
        <source>Type</source>
        <translation>类型</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="431"/>
        <source>Host Address</source>
        <translation>主机地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="441"/>
        <source>Host Port</source>
        <translation>主机端口</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="451"/>
        <source>Database Name</source>
        <translation>数据库实例</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="478"/>
        <source>User</source>
        <translation>用户名</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="488"/>
        <source>Password</source>
        <translation>口令</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="539"/>
        <source>Remove</source>
        <translation>移除</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="564"/>
        <location filename="zpmainframe.ui" line="1003"/>
        <source>Cluster</source>
        <translation>集群终端</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="582"/>
        <source>Terminal Listen Address</source>
        <translation>终端本地监听地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="592"/>
        <source>Listen Port</source>
        <translation>本地端口</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="632"/>
        <source>TransThreads</source>
        <translation>传输线程数</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="658"/>
        <source>Working Threads</source>
        <translation>处理线程数</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="686"/>
        <source>Active Terminals</source>
        <translation>可用集群终端表</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="704"/>
        <source>LAN Address</source>
        <translation>服务器局域网地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="714"/>
        <source>LAN Port</source>
        <translation>服务器局域网端口</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="748"/>
        <source>Publish Address</source>
        <translation>本节点对外发布地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="758"/>
        <source>Publish Port</source>
        <translation>发布端口</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1130"/>
        <source>Show&amp;Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1133"/>
        <source>Show main window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="572"/>
        <source>Publish Name</source>
        <translation>发布名称</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="737"/>
        <location filename="zpmainframe.ui" line="937"/>
        <source>&amp;Save</source>
        <translation>保存(&amp;S)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="781"/>
        <source>Participate</source>
        <translation>加入集群</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="795"/>
        <source>Example Server</source>
        <translation>范例服务器</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="877"/>
        <source>Cluster balance max payload</source>
        <translation>本服务器承担客户端超过门限后触发集群均衡</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1017"/>
        <source>Smartlink</source>
        <translation>Smartlink 专有设置</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="803"/>
        <source>Heart-beating</source>
        <translation>心跳检测最大保活时延</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="842"/>
        <source>seconds</source>
        <translation>秒</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="853"/>
        <source>User Account Database Name</source>
        <translation>用户信息数据库连接资源名称</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="863"/>
        <source>Main event db name</source>
        <translation>关键消息缓存数据库连接资源名称</translation>
    </message>
    <message>
        <source>Large data root folder</source>
        <translation type="obsolete">本地大文件存储根路径</translation>
    </message>
    <message>
        <source>...</source>
        <translation type="obsolete">...</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="955"/>
        <source>Messages</source>
        <translation>运行消息</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="975"/>
        <source>Nodes</source>
        <translation>节点</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1044"/>
        <source>&amp;Control</source>
        <translation>控制(&amp;C)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1053"/>
        <source>&amp;Help</source>
        <translation>帮助(&amp;H)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1084"/>
        <source>E&amp;xit</source>
        <translation>退出(&amp;x)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1097"/>
        <source>&amp;Start/Stop</source>
        <translation>开始/结束(&amp;S)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1100"/>
        <source>Start or stop the server</source>
        <translation>开启或者暂停服务</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1109"/>
        <source>&amp;About</source>
        <translation>关于(&amp;A)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1118"/>
        <source>&amp;Reload config file</source>
        <translation>重新加载配置文件(&amp;R)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1121"/>
        <source>Reload Config file</source>
        <translation>重新加载配置文件并调整服务器参数</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="152"/>
        <source>SSL</source>
        <translation>SSL</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="159"/>
        <source>HostAddr</source>
        <translation>服务地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="162"/>
        <source>Username</source>
        <translation>用户名</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="163"/>
        <source>Options</source>
        <translation>附加选项</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="164"/>
        <source>TestSQL</source>
        <translation>用于测试连接的SQL</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="177"/>
        <source>LAN_Address</source>
        <translation>服务器局域网地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="178"/>
        <source>LAN_Port</source>
        <translation>服务器局域网端口</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="179"/>
        <source>Clients</source>
        <translation>拥有客户端</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="180"/>
        <source>Pub_Address</source>
        <translation>本节点对外发布地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="181"/>
        <source>Pub_Port</source>
        <translation>发布端口</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="277"/>
        <source>Current Listen Threads: %1
</source>
        <translation>监听端口: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="279"/>
        <source>	Listen Threads %1: %2
</source>
        <translation>	线程%1 监听 %2 
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="282"/>
        <source>Current Trans Threads: %1
</source>
        <translation>传输线程: %1
</translation>
    </message>
    <message>
        <source>	Trans Threads %1 hold %2 Client Sockets.
</source>
        <translation type="obsolete">	传输线程 %1 负责 %2 个节点.
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="97"/>
        <source>Confire Exit</source>
        <translation>确认退出</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="98"/>
        <source>There are still some clients alive in the server. continue waiting?</source>
        <translation>有尚未主动退出的节点套接字，是否继续等待?</translation>
    </message>
    <message>
        <source>Address</source>
        <translation type="obsolete">发布地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="192"/>
        <location filename="zpmainframe.cpp" line="204"/>
        <location filename="zpmainframe.cpp" line="216"/>
        <location filename="zpmainframe.cpp" line="244"/>
        <source>,Source=%1</source>
        <translation>,来源:%1</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="286"/>
        <location filename="zpmainframe.cpp" line="329"/>
        <source>	%1:%2</source>
        <translation></translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="297"/>
        <source>Current Task Threads: %1
</source>
        <translation>逻辑处理线程: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="298"/>
        <source>Current Task Payload: %1
</source>
        <translation>逻辑处理负荷: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="299"/>
        <source>Current Task Idle Threads: %1
</source>
        <translation>空闲逻辑处理线程:%1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="303"/>
        <source>Database Connections: %1
</source>
        <translation>数据库连接: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="307"/>
        <source>	%1 status = %2</source>
        <translation>	连接 %1 状态 %2</translation>
    </message>
    <message>
        <source>Cluster Group Paras:
</source>
        <translation type="obsolete">集群终端参数:
</translation>
    </message>
    <message>
        <source>	Terminal %1 : %2, published Address: %3:%4
</source>
        <translation type="obsolete">	终端 %1 : %2, 发布地址: %3:%4
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="123"/>
        <location filename="zpmainframe.cpp" line="974"/>
        <source>Server still running</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="124"/>
        <location filename="zpmainframe.cpp" line="975"/>
        <source>If you want to terminate server, just using exit Toolbar button.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="314"/>
        <source>Cluster Paras:
</source>
        <translation>集群参数:
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="315"/>
        <source>	Terminal %1 : %2
		LAN Address: %3:%4
		 Publish: %5:%6
		balance max clients=%7
</source>
        <translation>	集群终端 %1:%2
		高速集群网地址 %3:%4
		发布公网地址  %5:%6
		集群均衡门限 %7
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="326"/>
        <source>Trans Threads: %1
</source>
        <translation>传输线程: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="334"/>
        <source>	Shift Threads: %1
</source>
        <translation>	处理线程: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="335"/>
        <source>	Shift Payload: %1
</source>
        <translation>	处理线程负荷: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="336"/>
        <source>	Shift Idle Threads: %1
</source>
        <translation>	处理线程空闲: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="339"/>
        <source>Smartlink Function Server Paras:
</source>
        <translation>Smartlink 功能服务器专有参数:
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="340"/>
        <source>	User Account Database is : %1
</source>
        <translation>	用户信息数据库: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="341"/>
        <source>	Event Database is : %1
</source>
        <translation>	重要事件数据库: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="342"/>
        <source>	Local folder for large file is : %1
</source>
        <translation>	磁盘仓库根路径: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="343"/>
        <source>	Heart beating Threadhold is : %1
</source>
        <translation>	心跳保活门限 %1 秒
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="373"/>
        <source> Rec %1B(%3 kbps)  Sent %2B(%4 kbps)</source>
        <translation>收 %1 字节(%3 kbps) 发 %2 字节(%4 kbps)</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="811"/>
        <source>Name Already Used.</source>
        <translation>名称已经存在.</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="811"/>
        <source>The listener name has been used.</source>
        <translation>监听器的名称已经被使用了。</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="816"/>
        <source>Invalid Paraments.</source>
        <translation>无效的参数.</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="816"/>
        <source>Address must be valid, Port between 1024 to 32767.</source>
        <translation>监听端口需要介于 1024 到 32767 之间。</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="862"/>
        <source>Open Conf file</source>
        <translation>打开配置文件</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="863"/>
        <source>Ini files(*.ini)</source>
        <translation>Ini文件(*.ini)</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="879"/>
        <source>Name can&apos;t be empty.</source>
        <translation>名称不能为空.</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="879"/>
        <source>Database name can not be empty.</source>
        <translation>数据库资源名称不能为空。</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="884"/>
        <source>Name already exist.</source>
        <translation>名称已经被使用了。</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="884"/>
        <source>Database name already exist.</source>
        <translation>名称已经被使用了。</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="981"/>
        <source>Server is  going to closed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="982"/>
        <source>Waiting for all unfinished progress...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ZPNetwork::zp_netListenThread</name>
    <message>
        <location filename="network/zp_netlistenthread.cpp" line="40"/>
        <source>Can not start listen!</source>
        <translation>无法启动监听！</translation>
    </message>
    <message>
        <location filename="network/zp_netlistenthread.cpp" line="66"/>
        <source>Listen Closed!</source>
        <translation>监听结束！</translation>
    </message>
</context>
<context>
    <name>ZPNetwork::zp_netTransThread</name>
    <message>
        <source>Client Accepted.</source>
        <translation type="vanished">接受节点的接入请求.</translation>
    </message>
    <message>
        <location filename="network/zp_nettransthread.cpp" line="179"/>
        <source>(%1)..Accepted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="network/zp_nettransthread.cpp" line="248"/>
        <source>Client connected.</source>
        <translation>节点已成功连接.</translation>
    </message>
    <message>
        <location filename="network/zp_nettransthread.cpp" line="250"/>
        <source>(%1)..connected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="network/zp_nettransthread.cpp" line="259"/>
        <source>(%1)..Encrypted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="network/zp_nettransthread.cpp" line="287"/>
        <source>(%1)..Closed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="network/zp_nettransthread.cpp" line="348"/>
        <source>(%1)..Error :%2.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="network/zp_nettransthread.cpp" line="369"/>
        <source>Client Error, Closed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Client Encrypted.</source>
        <translation type="vanished">节点已成功建立加密连接.</translation>
    </message>
    <message>
        <location filename="network/zp_nettransthread.cpp" line="286"/>
        <source>Client Closed.</source>
        <translation>节点已成功关闭.</translation>
    </message>
</context>
<context>
    <name>ZPNetwork::zp_net_Engine</name>
    <message>
        <location filename="network/zp_net_threadpool.cpp" line="117"/>
        <source>This ID has been used.</source>
        <translation>这个ID已经被使用了。</translation>
    </message>
    <message>
        <location filename="network/zp_net_threadpool.cpp" line="157"/>
        <source>Non-zp_netListenThread type detected.</source>
        <translation>非标准线程被检测到，服务器可能受到攻击.</translation>
    </message>
    <message>
        <location filename="network/zp_net_threadpool.cpp" line="162"/>
        <source>Incomming client arriverd.</source>
        <translation>收到一个节点接入申请.</translation>
    </message>
    <message>
        <source>New Client Arriverd.</source>
        <translation type="obsolete">新的节点接入。</translation>
    </message>
    <message>
        <location filename="network/zp_net_threadpool.cpp" line="192"/>
        <location filename="network/zp_net_threadpool.cpp" line="193"/>
        <location filename="network/zp_net_threadpool.cpp" line="460"/>
        <source>Need Trans Thread Object for clients.</source>
        <translation>需要一些逻辑处理线程来处理具体的事件。</translation>
    </message>
</context>
<context>
    <name>ZP_Cluster::zp_ClusterNode</name>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="229"/>
        <source>Client Send a unknown start Header %1 %2. Close client immediately.</source>
        <translation>客户发出了一个未知的开始头 %1 %2. 立刻关闭连接.</translation>
    </message>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="274"/>
        <source>Info: New Svr already regisited. Ignored.</source>
        <translation>Info: 这个节点已经注册过了,忽略请求.</translation>
    </message>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="285"/>
        <source>Can not connect to it-self, Loopback connections is forbidden.</source>
        <translation>请不要连接到自己的服务器节点. 回环连接是不被支持的.</translation>
    </message>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="312"/>
        <source>Name %1 &lt;= %2, omitted.</source>
        <translation>服务器 %1 优先级低于 %2，不发起连接。</translation>
    </message>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="338"/>
        <source>Info: Unknown Msg Type got.</source>
        <translation>Info: 未知的消息类型。</translation>
    </message>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="359"/>
        <source>Client </source>
        <translation>节点 </translation>
    </message>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="359"/>
        <source> is dead, kick out.</source>
        <translation>丢失心跳，终止该节点的连接。</translation>
    </message>
</context>
<context>
    <name>ZP_Cluster::zp_ClusterTerm</name>
    <message>
        <location filename="cluster/zp_clusterterm.cpp" line="206"/>
        <source>Name is empty!</source>
        <translation>注册的服务器名称为空！</translation>
    </message>
</context>
</TS>

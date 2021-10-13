## Websockt 连接   ws://8.210.124.139:16089

# 登录
## 邮箱密码登录(需要绑定邮箱)
```` c++
    //使用手机注册一个line 账号， 然后绑定一个邮箱，就可以用下面登录，第一次登陆需要在手机上输入pin，以后就直接使用cert 登录
    var json ={}
    json["command"] = "Login";
    json["email"] = "aaa@qq.com";
    json["password"] = "aaa";
    //第一次登陆的时候 为空字符串，需要在手机上输入Pin code， 登陆成功之后会收到回调，客户自己保存，下次直接使用
    json["cert"] = "";
    
    json["proxy_server"] = "";
    json["proxy_port"] = ;
    json["proxy_username"] = "";
    json["proxy_password"] = "";
    //ProxyType  socks5     http  http支持密码， socks5不支持密码
    json["proxy_type"] = "";

    websocket.send(JSON.stringify(json));
````
# Token 登录(手机号注册的时候提取的token)
```` c++
    var json ={}
    json["command"] = "LoginWithToken";
    json["phone"] = "+86adsfasf";
    json["auth_token"] = "xxxxx";
    json["proxy_server"] = "";
    json["proxy_port"] = ;
    json["proxy_username"] = "";
    json["proxy_password"] = "";
    //ProxyType  socks5     http  http支持密码， socks5不支持密码
    json["proxy_type"] = "http";

    websocket.send(JSON.stringify(json));
````

# 个人信息

## 获取配置
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "GetProfile";

````
## 获取加入的群
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "GetJoinedGroupIds";

````

## 获取所有好友id

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "GetAllContactIds";

````

## 获取好友信息

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "GetContacts";

    var mids = []
    mids.push("xxx");
    mids.push("bbb");

    json["mids"] = mids;

````

# 设置相关
## 打开/关闭好友通知

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "EnableContactNotification";

    json["mid"] = "xxxxx";
    json["enable"] = true

````

## 更新好友显示名称

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "UpdateContactName";

    json["mid"] = "xxxxx";
    json["name"] = "sdfafa";

````

## 星标好友

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "MarkContact";

    json["mid"] = "xxxxx";
    json["mark"] = true;

````

## 拉黑老友

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "BlockContact";

    json["mid"] = "xxxxx";
    json["block"] = true;

````

# 个人设置
## 设置昵称

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SetNickName";

    json["nickname"] = "xxxxx";

````
## 设置签名
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SetSignature";

    json["signature"] = "xxxxx";

````
## 设置头像
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "UpdateProfilePicture";

    json["path"] = "xxxxx";

````

## 检查user id 是否被占用

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "CheckUseridValid";

    json["user_id"] = "xxxxx";

````

## 设置userid

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SetMyUserid";

    json["user_id"] = "xxxxx";

````
## 获取自己的二维码
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "GetQrCode";

````

## 设置生日
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SetBirthday";
    json["year"] = "1980";
    json["day"] = "0811";

````
## 开启/关闭 user id 搜索
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "EnableSearchByUserId";
    json["enable"] = true;

````

## 开启/关闭 其他设备登录

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "EnableSecondaryLogin";
    json["enable"] = true;

````

## 获取设置
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "GetSettings";
    

````



# 搜索
## 搜索好友 userid
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SearchContactByUserid";
    json["user_id"] = "xxxxx";
````

## 手机号搜索
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SearchContactByPhones";

    var phones = [];
    phones.push("+86145656566");
    phones.push("+861898288929");

    json["phones"] = phones;

````

## url 搜索
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SearchContactByUrl";

    json["url"] = "https://line.me/R/ti/jutvcjvt1w";

````
# 添加好友
## mid 添加好友
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SearchAndAddContactsByMid";

    json["mid"] = "xxxx";

````




# 群操作
## url 搜索群
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SearchGroupByUrl";

    json["url"] = "https://line.me/R/ti/jutvcjvt1w";

````

## id 搜索群
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SearchGroupById";

    json["id"] = "118987297";

````

## 创建群
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "CreateGroup";

    json["name"] = "test";
    
    var mids = [];
    mids.push("xxxxx");
    mids.push("xxxxxx");

    json["mids"] = mids;

````

## 设置群头像
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "UpdateGroupPicture";

    json["groupId"] = "xxx";
    json["path"] = "xxx";

````


## 邀请入群
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "InviteIntoGroup";

    json["group_id"] = "11111111111";
    
    var mids = [];
    mids.push("xxxxx");
    mids.push("xxxxxx");

    json["mids"] = mids;

````
## 接受群邀请
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "AcceptGroupInvitation";

    json["group_id"] = "11111111111";
````

## 获取群二维码
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "GetGroupQrcode";

    json["group_id"] = "11111111111";
````

## 拒绝邀请

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "RejectGroupInvitation";

    json["group_id"] = "11111111111";
````

## 离开群

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "LeaveGroup";

    json["group_id"] = "11111111111";
````

## 加入群
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "JoinGroupByUrl";

    json["group_id"] = "11111111111";
    json["url"] = "afsdfsdf";
````

## 设置群名

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "UpdateGroupName";

    json["group_id"] = "11111111111";
    json["name"] = "test222";
````

## 开启/关闭 链接入群

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "EnableJoinByUrl";

    json["group_id"] = "11111111111";
    json["enable"] = true;
````


## 开启/关闭 群通知

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "EnableGroupNotification";

    json["group_id"] = "11111111111";
    json["enable"] = true;
````

# 消息
## 发送文本消息

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SendText";

    json["to"] = "xxxx";
    json["content"] = "hello";
````

## 发送定位消息

````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SendLocation";

    json["to"] = "xxxx";
    json["title"] = "title";
    json["address"] = "address";
    json["latitude"] = 45.773219;
    json["longitude"] = 126.657708;
````

## 发送联系人
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SendContact";

    json["to"] = "xxxx";
    json["displayName"] = "displayName";
    //联系人支持 mid  和 vcar， 通过 contactIsMid 决定传入的是哪种
    json["contactMidOrVcard"] = "contactMidOrVcard";
    json["contactIsMid"] = true;
````
## 发送图片
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SendImage";

    json["to"] = "xxxx";
    json["path"] = "xxx";
````
## 发送视频
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SendVideo";

    json["to"] = "xxxx";
    json["path"] = "xxx";
    json["duration"] = 6000;
````
## 发送音频
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SendAudio";

    json["to"] = "xxxx";
    json["path"] = "xxx";
    json["duration"] = 6000;
````
## 发送文件
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "SendFile";

    json["to"] = "xxxx";
    json["path"] = "xxx";
````
## 下载文件
````c++
    var json ={}
    json["task_id"] = task_id++;
    json["command"] = "DownloadFile";

    json["msg_id"] = "xxxx";
    json["path"] = "xxx";
````




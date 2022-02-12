package demo;

import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import io.netty.bootstrap.Bootstrap;
import io.netty.channel.Channel;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.http.DefaultHttpHeaders;
import io.netty.handler.codec.http.HttpClientCodec;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.websocketx.TextWebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketClientHandshakerFactory;
import io.netty.handler.codec.http.websocketx.WebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketVersion;
import io.netty.handler.codec.http.websocketx.extensions.compression.WebSocketClientCompressionHandler;
import io.netty.handler.ssl.SslContext;
import io.netty.handler.ssl.SslContextBuilder;
import io.netty.handler.ssl.util.InsecureTrustManagerFactory;
import websocket.WebSocketClientHandler;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.net.URI;
import java.util.*;
import java.util.Timer;

public class Demo extends JDialog implements WebSocketClientHandler.Handle {
    private JPanel contentPane;
    private JTextField email;
    private JTextField password;
    private JTextField cert;
    private JButton login;
    private JTextField userid;
    private JTextField mid;
    private JButton searchContact;
    private JButton getJoinedGroupsId;
    private JTextArea output;
    private JButton getAllContactIds;
    private JButton addByMid;
    private JButton getSettings;
    private JButton getQrcode;
    private JButton sendMessage;
    private JButton searchPhone;
    private JTextField phoneNumber;
    private JButton getMe;
    private JTextField contactmid;
    private JTextArea message;
    WebSocketClientHandler webSocketClientHandler;
    Channel websocketChannel;

    int taskId = 0;

    static final String URL = System.getProperty("url", "ws://8.210.124.139:16089");

    void SendMessage(JSONObject object) {
        WebSocketFrame frame = new TextWebSocketFrame(object.toString());
        websocketChannel.writeAndFlush(frame);
    }


    void ConnectServer() {
        try {
        URI uri = new URI(URL);
        String scheme = uri.getScheme() == null? "ws" : uri.getScheme();
        final String host = uri.getHost() == null? "127.0.0.1" : uri.getHost();
        final int port;
        if (uri.getPort() == -1) {
            if ("ws".equalsIgnoreCase(scheme)) {
                port = 80;
            } else if ("wss".equalsIgnoreCase(scheme)) {
                port = 443;
            } else {
                port = -1;
            }
        } else {
            port = uri.getPort();
        }

        if (!"ws".equalsIgnoreCase(scheme) && !"wss".equalsIgnoreCase(scheme)) {
            System.err.println("Only WS(S) is supported.");
            return;
        }

        final boolean ssl = "wss".equalsIgnoreCase(scheme);
        final SslContext sslCtx;
        if (ssl) {
            sslCtx = SslContextBuilder.forClient()
                    .trustManager(InsecureTrustManagerFactory.INSTANCE).build();
        } else {
            sslCtx = null;
        }

        EventLoopGroup group = new NioEventLoopGroup();

            // Connect with V13 (RFC 6455 aka HyBi-17). You can change it to V08 or V00.
            // If you change it to V00, ping is not supported and remember to change
            // HttpResponseDecoder to WebSocketHttpResponseDecoder in the pipeline.

            webSocketClientHandler = new WebSocketClientHandler(
                    WebSocketClientHandshakerFactory.newHandshaker(
                            uri, WebSocketVersion.V13, null, true, new DefaultHttpHeaders(), 10 * 1024 * 1024), this);

            Bootstrap b = new Bootstrap();
            b.group(group)
                    .channel(NioSocketChannel.class)
                    .handler(new ChannelInitializer<SocketChannel>() {
                        @Override
                        protected void initChannel(SocketChannel ch) {
                            ChannelPipeline p = ch.pipeline();
                            if (sslCtx != null) {
                                p.addLast(sslCtx.newHandler(ch.alloc(), host, port));
                            }
                            p.addLast(
                                    new HttpClientCodec(),
                                    new HttpObjectAggregator(65536),
                                    WebSocketClientCompressionHandler.INSTANCE,
                                    webSocketClientHandler);
                        }
                    });

            websocketChannel  = b.connect(uri.getHost(), port).sync().channel();
            webSocketClientHandler.handshakeFuture().sync();
        }
        catch (Exception e) {

        }
    }

    public Demo() {
        setContentPane(contentPane);
        setModal(true);

        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        setPreferredSize(new Dimension(1224,768));
        int frameWidth = this.getPreferredSize().width;
        int frameHeight = this.getPreferredSize().height;
        setSize(frameWidth, frameHeight);
        setLocation((screenSize.width - frameWidth) / 2,(screenSize.height - frameHeight) / 2);

        ConnectServer();

        // call onCancel() when cross is clicked
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                onCancel();
            }
        });

        // call onCancel() on ESCAPE
        contentPane.registerKeyboardAction(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                onCancel();
            }
        }, KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);
        login.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                //ty006009@outlook.com
                JSONObject command = new JSONObject();
                command.put("command", "Login");
                command.put("email", email.getText());
                command.put("password", password.getText());
                command.put("cert", cert.getText());
                SendMessage(command);
            }
        });
        searchContact.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                JSONObject command = new JSONObject();
                command.put("command", "SearchContactByUserid");

                command.put("user_id", userid.getText());
                command.put("task_id", taskId++);
                SendMessage(command);
            }
        });
        getJoinedGroupsId.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                JSONObject command = new JSONObject();
                command.put("command", "GetJoinedGroupIds");
                command.put("task_id", taskId++);
                SendMessage(command);
            }
        });
        getAllContactIds.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                JSONObject command = new JSONObject();
                command.put("command", "GetAllContactIds");
                command.put("task_id", taskId++);
                SendMessage(command);
            }
        });

        addByMid.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                JSONObject command = new JSONObject();
                command.put("command", "SearchAndAddContactsByMid");

                command.put("mid", mid.getText());
                command.put("task_id", taskId++);
                SendMessage(command);
            }
        });
        getSettings.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                JSONObject command = new JSONObject();
                command.put("command", "GetSettings");
                command.put("task_id", taskId++);
                SendMessage(command);
            }
        });
        getQrcode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                JSONObject command = new JSONObject();
                command.put("command", "GetQrCode");
                command.put("task_id", taskId++);
                SendMessage(command);
            }
        });
        sendMessage.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                JSONObject command = new JSONObject();
                command.put("command", "SendText");
                command.put("task_id", taskId++);
                command.put("to", contactmid.getText());
                command.put("content", message.getText());
                SendMessage(command);
            }
        });

        //手机号码前要带上 +
        searchPhone.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                JSONObject command = new JSONObject();
                command.put("command", "SearchContactByPhones");
                JSONArray phones = new JSONArray();
                phones.add(phoneNumber.getText());

                command.put("phones", phones);
                command.put("task_id", taskId++);
                SendMessage(command);
            }
        });
        getMe.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                JSONObject command = new JSONObject();
                command.put("command", "GetProfile");
                command.put("task_id", taskId++);
                SendMessage(command);
            }
        });
    }

    private void onOK() {
        // add your code here
        dispose();
    }

    private void onCancel() {
        // add your code here if necessary
        dispose();
    }

    public static void main(String[] args) {
        Demo dialog = new Demo();
        dialog.pack();
        dialog.setVisible(true);
        System.exit(0);
    }


    Timer heartBeatTimer;

    @Override
    public void OnConnect(int code, String msg) {
        SwingUtilities.invokeLater(() ->output.append(String.format("OnConnect:[%d:%s]\n", code, msg)));
        if (0 == code) {
            heartBeatTimer = new Timer();
            heartBeatTimer.schedule(new TimerTask() {
                @Override
                public void run() {
                    JSONObject command = new JSONObject();
                    command.put("command", "HeartBeat");
                    SendMessage(command);
                }
            }, 60 * 1000, 60 * 1000);
        }
    }

    @Override
    public void OnMessage(String text) {
        SwingUtilities.invokeLater(() ->output.append(String.format("OnMessage:[%s]\n", text)));
    }
}

import java.io.Serializable;

public class Message implements Serializable {

    private static final long serialVersionUID = 1L;
    private String nickName;
    private String content;

    public Message() {

    }

    public String getNickName() {
        return nickName;
    }

    public void setNickName(String nickName) {
        this.nickName = nickName;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }
}

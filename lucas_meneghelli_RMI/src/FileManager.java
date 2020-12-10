import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class FileManager {

    public Message readMessageIn(String fileName) {

        try {

            File myObj = new File(fileName);

            String content = "";

            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextLine()) {
                content += (content.length() > 0 ? "\n" : "") + myReader.nextLine();
            }
            myReader.close();

            String nickName = fileName.substring(0, fileName.indexOf('-'));

            Message message = new Message();
            message.setNickName(nickName);
            message.setContent(content);

            return message;

        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return null;
        }
    }

    public void writeContentInFile(String content, String fileName) {

        try {

            createFileWith(fileName);

            FileWriter myWriter = new FileWriter(fileName);
            myWriter.write(content);
            myWriter.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void createFileWith(String fileName) {

        try {

            File myObj = new File(fileName);
            myObj.createNewFile();

        } catch (IOException e) {
            e.printStackTrace();
        }

    }

}

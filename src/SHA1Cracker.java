import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


class KeyPresser {
    private char key;
    private int minTimesAllowed;
    private int maxTimesAllowed;
    private int noOfTimesPressed = 0;

    public KeyPresser(char key, int minTimesAllowed, int maxTimesAllowed) {
        if (maxTimesAllowed < minTimesAllowed) {
            throw new IllegalArgumentException("maxTimesAllowed can not be less than minTimesAllowed");
        }
        this.key = key;
        this.minTimesAllowed = minTimesAllowed;
        this.maxTimesAllowed = maxTimesAllowed;
        reset();
    }

    public void reset() {
        this.noOfTimesPressed = getMinTimesAllowed();
    }

    public int getNoOfTimesPressed() {
        return noOfTimesPressed;
    }

    public boolean canPress() {
        return noOfTimesPressed < maxTimesAllowed;
    }

    public void press() {
        if (!canPress()) {
            throw new IllegalAccessError("Can not press this key more than " + getMaxTimesAllowed() + " times");
        }
        this.noOfTimesPressed++;
    }

    public char getKey() {
        return key;
    }

    public int getMinTimesAllowed() {
        return minTimesAllowed;
    }

    public int getMaxTimesAllowed() {
        return maxTimesAllowed;
    }

    public String toString() {
        return "<"+getKey()+":"+getNoOfTimesPressed()+">";
    }
}

public class SHA1Cracker {
//    ExecutorService executor = Executors.newSingleThreadExecutor();


    public SHA1Cracker(List<KeyPresser> keyPressers) throws FileNotFoundException {
        try {
            tryCombinations(keyPressers, 0);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    File createOutputFile(String filename) throws IOException {
        String path = "D:" + File.separator + "abhinav" + File.separator + filename;
        File f = new File(path);

        f.getParentFile().mkdirs();
        f.createNewFile();
        return f;
    }

    void tryCombinations(List<KeyPresser> keyPressers, int currentIndex) throws IOException {
        if (currentIndex == keyPressers.size()) {
            int totalChars = 0;

            for (KeyPresser keyPresser : keyPressers) {
                totalChars += keyPresser.getNoOfTimesPressed();
            }

            char charactersInPassword[] = new char[totalChars];
            int i = 0;
            for (KeyPresser keyPresser : keyPressers) {
                for (int j = 0; j < keyPresser.getNoOfTimesPressed(); j++) {
                    charactersInPassword[i++] = keyPresser.getKey();
                }
            }

            File fout = createOutputFile("result-"+ new String(charactersInPassword) +".txt");
            FileOutputStream fos = new FileOutputStream(fout);
            OutputStreamWriter osw = new OutputStreamWriter(fos);
            System.out.println("Writing results to : " + fout.getAbsolutePath());

            File statsFile = new File("stats-"+new String(charactersInPassword)+".txt");
            FileOutputStream statsStream = new FileOutputStream(statsFile);
            OutputStreamWriter statsWriter = new OutputStreamWriter(statsStream);
            System.out.println("Writing stats to : " + statsFile.getAbsolutePath());

            new PasswordGenerator(charactersInPassword, osw, statsWriter).run();
            return;
        }

        KeyPresser keyPresser = keyPressers.get(currentIndex);

        while (true){
            tryCombinations(keyPressers, currentIndex + 1);
            if (keyPresser.canPress())
                keyPresser.press();
            else
                break;
        }
        keyPresser.reset();
    }

    public static void main(String[] args) throws FileNotFoundException {
        List<KeyPresser> keyPressers = new ArrayList<>();
        keyPressers.add(new KeyPresser('q', 2, 3));
        keyPressers.add(new KeyPresser('Q', 2, 3));
        keyPressers.add(new KeyPresser('w', 2, 3));
        keyPressers.add(new KeyPresser('W', 2, 3));
        keyPressers.add(new KeyPresser('5', 2, 3));
        keyPressers.add(new KeyPresser('%', 2, 3));
        keyPressers.add(new KeyPresser('n', 2, 3));
        keyPressers.add(new KeyPresser('N', 2, 3));
        keyPressers.add(new KeyPresser('8', 2, 3));
        keyPressers.add(new KeyPresser('(', 2, 3));
        keyPressers.add(new KeyPresser('i', 2, 3));
        keyPressers.add(new KeyPresser('I', 2, 3));
        keyPressers.add(new KeyPresser('0', 2, 3));
        keyPressers.add(new KeyPresser('=', 2, 3));

        new SHA1Cracker(keyPressers);
    }
}

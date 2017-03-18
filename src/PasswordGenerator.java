import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class PasswordGenerator implements Runnable{
    private BufferedWriter resultWriter;
    private BufferedWriter statsWriter;
    private char originalChars[];
    private char chars[];

    public PasswordGenerator(char chars[], OutputStreamWriter resultWriter, OutputStreamWriter statsWriter) {
        this.resultWriter = new BufferedWriter(resultWriter);
        this.statsWriter = new BufferedWriter(statsWriter);
        this.originalChars = chars.clone();
        this.chars = chars;
    }

    // A utility function two swap two characters a and b
    void swap(char[] str, int i, int j)
    {
        char t = str[i];
        str[i] = str[j];
        str[j] = t;
    }

    // This function finds the index of the smallest character
    // which is greater than 'first' and is present in str[l..h]
    int findCeil(char str[], char first, int l, int h)
    {
        // initialize index of ceiling element
        int ceilIndex = l;

        // Now iterate through rest of the elements and find
        // the smallest character greater than 'first'
        for (int i = l+1; i <= h; i++)
            if (str[i] > first && str[i] < str[ceilIndex])
                ceilIndex = i;

        return ceilIndex;
    }

    void cleanup() {
        try {
            resultWriter.close();
        } catch (IOException e1) {
            e1.printStackTrace();
        }
        try {
            statsWriter.close();
        } catch (IOException e1) {
            e1.printStackTrace();
        }
    }

    void log(long index) throws IOException {
        statsWriter.write(String.format("%s - %d\n", new String(originalChars) , index));
    }

    @Override
    public void run() {
        List<String> permutations = new ArrayList<>();
        // Get size of string
        int size = chars.length;

        // Sort the string in increasing order
        Arrays.sort(chars);
//        qsort(str, size, sizeof( str[0] ), compare);
        long count = 0;

        // Print permutations one by one
        boolean isFinished = false;
        while (!isFinished)
        {
            try {
                count++;
                resultWriter.write(chars);
                resultWriter.write('\n');
                if (count%1000 == 0) {
                    log(count);
                }
            } catch (IOException e) {
                e.printStackTrace();
                cleanup();
                break;
            }
            // Find the rightmost character which is smaller than its next
            // character. Let us call it 'first char'
            int i;
            for (i = size - 2; i >= 0; --i)
                if (chars[i] < chars[i+1])
                    break;

            // If there is no such chracter, all are sorted in decreasing order,
            // means we just printed the last permutation and we are done.
            if (i == -1)
                isFinished = true;
            else
            {
                // Find the ceil of 'first char' in right of first character.
                // Ceil of a character is the smallest character greater than it
                int ceilIndex = findCeil(chars, chars[i], i + 1, size - 1);

                // Swap first and second characters
                swap(chars, i, ceilIndex);

                // Sort the string on right of 'first char'
                Arrays.sort(chars, i+1, size);
//                qsort(str + i + 1, size - i - 1, sizeof(str[0]), compare);
            }
        }
        try {
            log(count);
        } catch (IOException e) {
            e.printStackTrace();
        }
        cleanup();
    }
}

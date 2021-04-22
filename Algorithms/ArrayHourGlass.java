import java.io.*;
import java.math.*;
import java.security.*;
import java.text.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.regex.*;

public class Solution {

    // Complete the hourglassSum function below.
    static int hourglassSum(int[][] arr) {
        int largestValue = Integer.MIN_VALUE;
        //iterate over 2D array
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
                
                int curSum = 0;
                //iterarte over hourglass shape
                for(int i = 0; i < 3; i++){
                    for(int j = 0; j < 3; j++){
                        
                        if(i == 1 && j != 1){ //if 2nd row of hour glass
                            continue;
                        }
                        else{
                            curSum += arr[i + row][j + col];
                        }
                    }
                }
                if(curSum > largestValue){
                    largestValue = curSum;
                }
            }
        }
        return largestValue;
    }

    private static final Scanner scanner = new Scanner(System.in);

    public static void main(String[] args) throws IOException {
        BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(System.getenv("OUTPUT_PATH")));

        int[][] arr = new int[6][6];

        for (int i = 0; i < 6; i++) {
            String[] arrRowItems = scanner.nextLine().split(" ");
            scanner.skip("(\r\n|[\n\r\u2028\u2029\u0085])?");

            for (int j = 0; j < 6; j++) {
                int arrItem = Integer.parseInt(arrRowItems[j]);
                arr[i][j] = arrItem;
            }
        }

        int result = hourglassSum(arr);

        bufferedWriter.write(String.valueOf(result));
        bufferedWriter.newLine();

        bufferedWriter.close();

        scanner.close();
    }
}

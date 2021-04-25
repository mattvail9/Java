import java.io.*;
import java.math.*;
import java.security.*;
import java.text.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.regex.*;

public class Solution {

    // Complete the minimumSwaps function below.
    static int minimumSwaps(int[] arr) {
        int numSwaps = 0;
        //create a copy of the initil array 
        int[] copy = new int[arr.length];
        for(int i = 0; i < arr.length; i++){
            copy[i] = arr[i];
        }
        //sort the copy so the elements are in the correct order
        Arrays.sort(copy);
        
        /* loop through the initial array
        if the element at i is not in the correct position
        swap it with index of the element that should be in 
        positon i */
        for(int i = 0; i < arr.length; i++){
            if(arr[i] != copy[i]){
                numSwaps++;
                swap(arr, i, indexOf(arr, copy[i]));      
            }
        }
        return numSwaps;
    }
    
    static void swap(int arr[], int i, int j){
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;    
    }
    
    static int indexOf(int[] arr, int element){
        for(int i = 0; i < arr.length; i++){
            if(arr[i] == element){
                return i;
            }
        }
        return -1;
    }
    
    private static final Scanner scanner = new Scanner(System.in);

    public static void main(String[] args) throws IOException {
        BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(System.getenv("OUTPUT_PATH")));

        int n = scanner.nextInt();
        scanner.skip("(\r\n|[\n\r\u2028\u2029\u0085])?");

        int[] arr = new int[n];

        String[] arrItems = scanner.nextLine().split(" ");
        scanner.skip("(\r\n|[\n\r\u2028\u2029\u0085])?");

        for (int i = 0; i < n; i++) {
            int arrItem = Integer.parseInt(arrItems[i]);
            arr[i] = arrItem;
        }

        int res = minimumSwaps(arr);

        bufferedWriter.write(String.valueOf(res));
        bufferedWriter.newLine();

        bufferedWriter.close();

        scanner.close();
    }
}

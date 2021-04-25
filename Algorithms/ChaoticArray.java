import java.io.*;
import java.math.*;
import java.security.*;
import java.text.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.function.*;
import java.util.regex.*;
import java.util.stream.*;
import static java.util.stream.Collectors.joining;
import static java.util.stream.Collectors.toList;

class Result {

    /*
     * Complete the 'minimumBribes' function below.
     *
     * The function accepts INTEGER_ARRAY q as parameter.
     */

    public static void minimumBribes(List<Integer> q) {
    // Write your code here
        int numBribes = 0;
        boolean tooChaotic = false;
        for(int i = 0; i < q.size(); i++){
            /*i is the origional position of each person
            personNum is the position after being bribed n numbers of times
            subtracting 1 matches the values to the actual array index */
            int personNum = q.get(i) - 1;
            
            /* if the person is more than two positions away from
            their origional position then the array is 'too chaotic' */
            if(personNum - i > 2){
                tooChaotic = true;
                break;
            }
            /* To make sure the index being checked does not go below 0,
            use the Math.max function.
            Loop through from one in front of persons origional position
            to one in front of their origional position. */
            for(int j = Math.max(personNum - 1, 0); j < i; j++){
                if(q.get(j) > personNum){
                    numBribes++;
                }
            }
        }
        
        if(tooChaotic){
            System.out.println("Too chaotic");
        }
        else{
            System.out.println(numBribes);
        }
        
    }

}

public class Solution {
    public static void main(String[] args) throws IOException {
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(System.in));

        int t = Integer.parseInt(bufferedReader.readLine().trim());

        IntStream.range(0, t).forEach(tItr -> {
            try {
                int n = Integer.parseInt(bufferedReader.readLine().trim());

                List<Integer> q = Stream.of(bufferedReader.readLine().replaceAll("\\s+$", "").split(" "))
                    .map(Integer::parseInt)
                    .collect(toList());

                Result.minimumBribes(q);
            } catch (IOException ex) {
                throw new RuntimeException(ex);
            }
        });

        bufferedReader.close();
    }
}

import java.util.*;
import java.io.*;
 
/* This program checks if any subset of an array sums to the largest value in the array */
class SubSetSum {

  /* Recursive function that uses backtrcking to check the sum of any subset equals the target value */
	static boolean isSubSet(int[] set, int n, int target){

    if (target == 0)
        return true; //sum has been found
    if (target < 0 || n == 0)
        return false; //sum has not been found

    //recurse through remaining susets
    return isSubSet(set, n - 1, target)
        || isSubSet(set, n - 1, target - set[n - 1]);
	}
	
  //Driver function
	public static void main(String[] args){
		int[] set = { 5, 7, 16, 1, 2 };
    
    //loop through array to find the largets value
    int largestNum = set[0];
		for(int i = 1; i < set.length; i++){
			if(set[i] > largestNum){
				largestNum = set[i];
			}
		}
		
    //remove the largest value by constructing a new array
		int[] newArr = new int[set.length - 1];
		for(int i = 0, k = 0; i < set.length; i++){
			if(set[i] != largestNum){
				newArr[k++] = set[i];
			}
		}
		
    int n = newArr.length;
    if (isSubSet(newArr, n, largestNum) == true)
        System.out.println("Found a subset with target sum");
    else
        System.out.println("No subset with target sum");
	}
}

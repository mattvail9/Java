//A Finite Automaton
//Accepts Language (00|11)*
public class FSM
{
	  private static char state '-1';
	
	  public static void main(String[] args) {
		String stringA = "001100110011";
		String stringB = "110011001001";
		System.out.printf("FA accepts %s? %b", stringA, accepts(stringA));
		System.out.printf("FA accepts %s? %b", stringB, accepts(stringB));
	}
	
	private static boolean accepts(String w) 
	{
		for (char ch : w.toCharArray()) 
		{
			if(state == '-1')
			{
				//set state to current input
				state = ch;			    //1x | 0x
			}
			else if(state == '0')	//0x
			{
				if(ch == '0')
					state = '-1';	    //00	reset state
				else
					return false;	    //01	invalid string
			}
			else if(state == '1')	//1x
			{
				if(ch == '1')
					state = '-1';	    //11	reset state
				else
					return false;	    //10	invalid string			
			}
		}
		return true;
	}
}

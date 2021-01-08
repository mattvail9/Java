public class AnswerKey
{
	public String question = "";
	public int correctAnswerIndex;
	public String[] answers;
	public int userAnswer = -1;
	public int correctAnswer = -2;
	
	public AnswerKey()
	{
		
	}
	public AnswerKey(String question, int correctAnswerIndex, String... answers)
	{
		this.question = question;
		this.correctAnswerIndex = correctAnswerIndex;
		this.answers = answers;
	}
	
	public AnswerKey(String question, int userAnswer, int correctAnswer)
	{
		this.question = question;
		this.userAnswer = userAnswer;
		this.correctAnswer = correctAnswer;
	}
	public AnswerKey(int userAnswer, int correctAnswer)
	{
		this.userAnswer = userAnswer;
		this.correctAnswer = correctAnswer;
	}
}
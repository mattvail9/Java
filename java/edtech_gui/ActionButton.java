import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.border.LineBorder;

public class ActionButton extends JLabel
{
	private int defaultBorderSize = 1;
	private int buttonSize = 32;
	
	public ActionButton(String btnText){
		super(btnText, SwingConstants.CENTER);
		setBorder( new LineBorder(Color.pink, defaultBorderSize) );
		setForeground( Color.white );
		setOpaque(true);
		setBackground(Color.black);
		addMouseListener(new MouseAdapter() {
			public void mouseEntered(MouseEvent evt) {
				setBorder( new LineBorder(Color.white, 1) );
				setBackground( Color.darkGray );
			}
			public void mouseExited(MouseEvent evt) {
				setBorder( new LineBorder(Color.pink, defaultBorderSize) );
				setBackground(Color.black);
			}
		});
	}
	
	public void addIcon(String path){
		setIcon(Resource_Manager.loadResourceScaled(path, buttonSize, buttonSize));
	}
}
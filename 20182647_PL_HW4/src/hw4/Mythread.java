package hw4;

import javax.swing.JOptionPane;

public class Mythread extends Thread{
	String path;
	String pattern;
	
	public Mythread(String path, String pattern)
	{
		this.path = path;
		this.pattern = pattern;
	}
	
	public void run()
	{
		try{
			newWindow new_window = new newWindow(pattern);
			int result = new_window.search(path);
			if(result == 1)
			{
				JOptionPane.showMessageDialog(null, "검색완료!");
				return;
			}
			System.out.println("search end!");
		}
		catch(Exception e)
		{
	
			return;
		}
	}
}

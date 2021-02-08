package hw4;

import java.awt.BorderLayout;
import java.awt.EventQueue;
import java.awt.FlowLayout;
import java.awt.Container;
import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.ScrollPaneConstants;
import javax.swing.border.EmptyBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.text.html.HTMLDocument.Iterator;
import javax.swing.BoxLayout;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.StringTokenizer;
import java.util.Vector;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.awt.event.ActionEvent;

class newWindow extends JFrame
{	
	boolean flag = false;
	Container c;
	Pattern regular_pattern;
	Pattern regular_pattern_extension;
	Vector<String> filenames = new Vector<String>();
	String selectedData = new String();
	
	JPanel panel_file_info;
	JScrollPane scroll_area;
	JList<String> list;
	
	JPanel current_folder_info;
	JLabel current_info;
	
	JPanel buttons;
	JButton delete;
	JButton stop;
	public newWindow(String pattern)
	{
		setTitle("새로운 창");
		setSize(1200, 1000);
			
		c = getContentPane();
		c.setLayout(new BoxLayout(c, BoxLayout.Y_AXIS));
		
		//현재 정보
		current_folder_info = new JPanel();
		current_folder_info.setLayout(new BoxLayout(current_folder_info, BoxLayout.X_AXIS));
		current_info = new JLabel("현재 : ");
		current_folder_info.add(current_info);
		
		//파일 정보들
		list = new JList<String>(filenames);
		list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		list.addListSelectionListener(new ListSelectionListener() {
			public void valueChanged(ListSelectionEvent e)
			{
				if(!e.getValueIsAdjusting())
				{
					selectedData = list.getSelectedValue();
				}
			}
		});
		scroll_area = new JScrollPane(list);
		scroll_area.setPreferredSize(new Dimension(800,800));
		
		//파일삭제, 검색취소 버튼 패널
		buttons = new JPanel();
		buttons.setLayout(new BoxLayout(buttons, BoxLayout.X_AXIS));
		delete = new JButton("파일삭제");
		delete.addActionListener(new ActionListener() {
			synchronized public void actionPerformed(ActionEvent e)
			{
				if(selectedData.length() == 0)
				{
					JOptionPane.showMessageDialog(null, "삭제할 파일을 선택하세요!");
					return;
				}
				
				StringTokenizer token1 = new StringTokenizer(selectedData, "|");
				String selectedData_absolutePath = new String();
				Vector<String> datas = new Vector<String>();
					
				while(token1.hasMoreTokens())
				{
					datas.add(token1.nextToken());
				}
				
				selectedData_absolutePath = datas.get(2);
				File deleteFile = new File(selectedData_absolutePath);
				if(deleteFile.isFile())
				{
					deleteFile.delete();
					
					for(int i=0; i<filenames.size(); i++)
					{
						if(selectedData.equals(filenames.get(i)))
						{
							filenames.remove(i);
							list.setListData(filenames);
							break;
						}
					}
				}
			}
		});
		stop = new JButton("검색취소");
		stop.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{
				flag = true;
			}
		});
		buttons.add(delete);
		buttons.add(stop);
		
		c.add(current_folder_info);
		c.add(scroll_area);
		c.add(buttons);
		setVisible(true);
		
		StringBuilder name = new StringBuilder();
		int n = pattern.length();
		for(int i=0; i<n; i++)
		{
			if(pattern.charAt(i) == '?')
				name.append(".");
			else if(pattern.charAt(i) == '*')
				name.append(".*");
			else
			{
				if(pattern.charAt(i) == '.' || pattern.charAt(i) == '$')
				{
					name.append("\\" + pattern.charAt(i));
				}
				else
				{
					String temp = Character.toString(pattern.charAt(i));
					name.append(temp);
				}
			}
		}
		regular_pattern = Pattern.compile("^" + name + "$");
	}

	public int search(String str)
	{
		if(flag == true) return 0;
		
		File dir = new File(str);
		File[] fileList = dir.listFiles();
		current_info.setText("현재 : " + str);
		
		try
		{
			for(int i=0; i<fileList.length; i++)
			{
				if(flag == true) return 0;
				File file = fileList[i];
				Thread.sleep(10);
				if(file.isDirectory())
				{
					search(file.getAbsolutePath());
					continue;
				}
				
				Matcher m = regular_pattern.matcher(file.getName());
				
				if(m.find() == false)
				{
					continue;
				}		
				
				long lastModified = file.lastModified();

				String date_pattern = "yyyy-MM-dd hh:mm aa";
				SimpleDateFormat simpleDateFormat = new SimpleDateFormat(date_pattern);
				
				Date lastModifiedDate = new Date( lastModified );
				filenames.add(file.getName() + "|" + file.length() + "|" +  file.getAbsolutePath() + "|" + simpleDateFormat.format( lastModifiedDate ));
				list.setListData(filenames);
			}
		}
		catch(Exception e)
		{
			
		}
		return 1;
	}
}

public class MainWindow extends JFrame {
	JTextField pathname;
	JTextField search_text;
	
	public static void main(String[] args) {
		new MainWindow();
	}
	
	public boolean check_absolute()
	{
		String path = pathname.getText();
		String start_absol = "C:\\";
		
		if(path.length() < 3) return false;
		
		int flag = 0;
		for(int i=0; i<3; i++)
		{
			if(path.charAt(i) != start_absol.charAt(i))
			{
				flag = 1;
				return false;
			}
		}
		return true;
	}
	
	public boolean check_error(String search)
	{
		StringTokenizer tokens = new StringTokenizer(search, ".");
		Vector<String> search_vec = new Vector<String>();
		String extension = new String();
		
		while(tokens.hasMoreTokens())
			search_vec.add(tokens.nextToken());
		
		if(search_vec.size() == 1)
			return true;
		
		extension = search_vec.get(search_vec.size() - 1);
		int flag = 0;
		for(int i=0; i<extension.length(); i++)
		{
			if(extension.charAt(i) == '?')
			{
				flag = 1;
				return false;
			}
		}
		return true;
	}
	
	public boolean check_forbidden_letter(String data, int flag)
	{
		int ret = 0;
		//경로
		if(flag == 0)
		{
			StringTokenizer token = new StringTokenizer(data, "\\");
			
			while(token.hasMoreTokens())
			{
				String tmp = token.nextToken();
				
				int check_absolute = tmp.compareTo("C:");
				
				if(check_absolute == 0)
					continue;
					
				for(int i=0; i<tmp.length(); i++)
				{
					if(tmp.charAt(i) == '/' || tmp.charAt(i) == ':' ||
							tmp.charAt(i) == '\"' || tmp.charAt(i) == '>' || tmp.charAt(i) == '<' ||
									tmp.charAt(i) == '|' || tmp.charAt(i) == '?')
					{
						return false;
					}
				}
			}
			return true;
		}
		//검색어
		else
		{
			for(int i=0; i<data.length(); i++)
			{
				if(data.charAt(i) == '\\' || data.charAt(i) == '/' || data.charAt(i) == ':' ||
						data.charAt(i) == '\"' || data.charAt(i) == '>' || data.charAt(i) == '<' ||
						data.charAt(i) == '|')
				{
					return false;
				}
			}
			return true;
		}
	}
	
	public MainWindow() {
		setTitle("프로그래밍언어 과제4");
		setSize(500, 150);
		pathname = new JTextField();
		search_text = new JTextField();
		JButton exit_bt = new JButton("종료");
		exit_bt.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{
				System.exit(0);
			}
		});
		JButton exec_bt = new JButton("검색");
		exec_bt.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e)
			{

				try
				{
					if(pathname.getText().length() == 0)
					{
						JOptionPane.showMessageDialog(null, "경로명을 입력하세요!");
						return;
					}	
					if(search_text.getText().length() == 0)
					{
						JOptionPane.showMessageDialog(null, "검색어를 입력하세요!");
						return;
					}
					
					File check_exist = new File(pathname.getText());
					
					if(check_exist.exists() == false)
					{
						JOptionPane.showMessageDialog(null, "존재하지 않는 경로!");
						return;
					}
				
					if(check_absolute() == false)
					{
						JOptionPane.showMessageDialog(null, "절대경로를 입력하세요!");
						return;
					}
					
					if(check_error(search_text.getText()) == false)
					{
						JOptionPane.showMessageDialog(null, "검색어 확장자에는 ? 사용 불가!!!!");
						return;
					}
					
					if(check_forbidden_letter(pathname.getText(), 0) == false)
					{
						JOptionPane.showMessageDialog(null, "경로명에 올바르지 않은 문자 존재!!");
						return;
					}
					if(check_forbidden_letter(search_text.getText(), 1) == false)
					{
						JOptionPane.showMessageDialog(null, "검색어에 올바르지 않은 문자 존재!!");
						return;
					}
				}
				catch(Exception err)
				{
					err.printStackTrace();
				}
							
				Mythread test = new Mythread(pathname.getText(), search_text.getText());
				test.start();
			}
		});
			
		Container c = getContentPane();
		c.setLayout(new BoxLayout(c, BoxLayout.Y_AXIS));
		
		JPanel panel1 = new JPanel();
		panel1.setLayout(new BoxLayout(panel1, BoxLayout.X_AXIS));
		panel1.add(new JLabel("경로명 : "));
		panel1.add(pathname);
		
		JPanel panel2 = new JPanel();
		panel2.setLayout(new BoxLayout(panel2, BoxLayout.X_AXIS));
		panel2.add(new JLabel("검색어 : "));
		panel2.add(search_text);
		
		JPanel panel3 = new JPanel();
		panel3.setLayout(new FlowLayout());
		panel3.add(exec_bt);
		panel3.add(exit_bt);
		
		
		c.add(panel1);
		c.add(panel2);
		c.add(panel3);
		
		setVisible(true);
	}
	
}

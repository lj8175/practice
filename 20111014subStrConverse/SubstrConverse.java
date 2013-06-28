
public class SubstrConverse {
	
	
	public SubstrConverse(String s){
		str = new StringBuffer(s);
	}
	
	//删除字符串中的空格
	public void deleteSpace(){
		if(str.length() == 0) return;
		StringBuffer arr = new StringBuffer();
		boolean spaceflag = false;
		for(int i=0; i<str.length(); i++){
			if(str.charAt(i) != ' '){
				arr.append(str.charAt(i));
				spaceflag = false;
			}
			else if(!spaceflag){
				arr.append(str.charAt(i));
				spaceflag = true;
			}
		}		
		str = arr;
	}
	//对字符串中的各字串进行逆置
	public void subConverse(){
		int fromIndex = 0;
		for(int i=0; i<str.length(); i++){
			if(str.charAt(i) != ' '){
				fromIndex = i;
				break;
			}
		}
		int subend=0;
		while((subend = str.indexOf(" ", fromIndex)) != -1){
			converse(fromIndex, subend-1);
			fromIndex = subend +1;
		}
		if(fromIndex != str.length())
			converse(fromIndex, str.length()-1);
	}
	//辅助subConverse，对取出的一个字串进行逆置
	private void converse(int begin, int end){
		while(begin < end){
			char temp = str.charAt(begin);
			str.setCharAt(begin, str.charAt(end));			
			str.setCharAt(end, temp);
			begin++;
			end--;
		}
	}
	
	public void print(){
		System.out.println(str);
	}
	
	private StringBuffer str;
	
	
	
	public static void main(String[] args){
		SubstrConverse sc = new SubstrConverse("  abcd  cde  efg  ");
		sc.deleteSpace();
		sc.subConverse();
		sc.print();
	}

}

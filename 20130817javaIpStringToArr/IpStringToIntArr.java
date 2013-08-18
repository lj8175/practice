
public class IpStringToIntArr {
	
	public static int getIPIntArr(String ip_string, int[] ip_int) {
		int ret = 0;
		if (ip_string.charAt(ip_string.length() - 1) - '.' == 0)
			return -1;
		int dotInt = '.' - '0';
		int ip_each_int = 0;
		int m = 0;
		for (int i = 0, j = i; i < ip_string.length(); i++) {
			if (m >= ip_int.length)
				return -2;
			int tmp = ip_string.charAt(i) - '0';
			if (tmp == 0) {
				if (i == j && (i != ip_string.length() - 1)
						&& (ip_string.charAt(i + 1) - '.' != 0)) {
					return -3;
				} else {
					ip_each_int = ip_each_int * 10 + tmp;
				}
			} else if (tmp > 0 && tmp <= 9) {
				ip_each_int = ip_each_int * 10 + tmp;
			} else if (tmp == dotInt) {
				if (ip_each_int >= 0 && ip_each_int < 255) {
					ip_int[m++] = ip_each_int;
					ip_each_int = 0;
					j = i + 1;
				} else {
					return -4;
				}
			}
			if (i == ip_string.length() - 1) {
				ip_int[m++] = ip_each_int;
			}
		}
		if (m < ip_int.length)
			return -5;
		if (ip_int[0] == 0)
			return -6;
		return ret;
	}
	
	public static int IpStringToArr(String ipStr, int[] intArr)
	{
		if(ipStr.length()<8 || ipStr.length()>15 || intArr.length<4)
			return -1;
		int dotIndexOld=0, dotNum=0;
		if(ipStr.charAt(0)=='0') return -1;
		for(int i=0; i<ipStr.length(); i++)
		{
			if(ipStr.charAt(i)=='.')
			{
				dotNum++;
				if((i-dotIndexOld)<=0||(i-dotIndexOld)>4) return -1;
				if((i-dotIndexOld)>2 && ipStr.charAt(dotIndexOld+1)=='0') return -1;
				dotIndexOld = i;
			}
			else
			{
				int num = ipStr.charAt(i) - '0';
				if(num<0||num>9)
					return -1;
			}
		}
		if(dotNum!=3) return -1;
		
		int tmp=0,index = 0;
		for(int i=0; i<ipStr.length(); i++)
		{
			int num = ipStr.charAt(i) - '0';
			if(ipStr.charAt(i)=='.')
			{
				intArr[index] = tmp;
				index++;
				tmp = 0;
				continue;
			}
			tmp = tmp*10+num;
		}
		intArr[index] = tmp;
		
		for(int i=0; i<4; i++)
		{
			if(intArr[i]>255) return -1;
		}
		return 0;
	}

	public static void main(String[] args) {
		String ipStr = "02.01.192.01";
		//ipStr = ".01.192.01.";
		//ipStr = "1.192.01.78.";
		ipStr = "1.192.1.78";
		int[] intArr= new int[4];
		int ret = IpStringToArr(ipStr, intArr);
		//ret = getIPIntArr(ipStr, intArr);
		System.out.println(ret);
		for (int i=0; i<intArr.length; i++)
		{
			System.out.print(intArr[i]+" ");
		}
		System.out.println();
		

	}

}

import java.io.*;

public class SongConverter {

	public static void main(String[] args) {
		BufferedReader rd=new BufferedReader(new InputStreamReader(System.in));
		try {
			while (rd.ready()) {
				String[] cols=new String[0];
			
				cols=rd.readLine().split(" ");
				if (cols.length>1) {
					for (int i=2;i<6;i++) {
						//System.out.println (cols[i]);
						for (int j=0;j<8;j++) {
							if (j==cols[i].length()) {
								System.out.println();
								return;
							}
							if (j%2==0) System.out.print ("0x");
							if (j<cols[i].length()) System.out.print(cols[i].charAt(j));
							if (j%2==1) System.out.print (",");
						}
					}
					System.out.println();
				}
			}
		} catch (Exception e) {e.printStackTrace();}
	}

}

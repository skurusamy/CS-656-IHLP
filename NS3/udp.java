
import java.io.IOException;
import java.io.File;
import java.io.FileInputStream;
import java.net.InetAddress;
import java.net.DatagramPacket;
import java.net.DatagramSocket;



public class udp{
	private static final int DELAY=0;
	private static final int BUFFERSIZE=256;

	public static void main(String[] args) throws IOException, NumberFormatException,InterruptedException
	{
	  try{
	  //checks comment line argument
	    if(args.length > 5 ||args.length<3|| args[0]==null || args[1]==null||args[2]==null)
	  	{
		  System.out.println("Insufficient Arguments\n");
		  System.exit(0);
	        }
		//valid port number
	     int port = Integer.parseInt(args[1]);
	     if(port > 65535 || port <=1024)
	      {
		  System.out.println("Invalid Port Number\n");
		  System.exit(0);
	      }

	      //Converts mp3 to Byte
	      String path = args[2];
	      File fp = new File(path);
	      FileInputStream file_in = new FileInputStream(fp);
	      int fileLength = (int) fp.length();
	      byte[] audio = new byte[fileLength];
	      file_in.read(audio,0,fileLength);
	      file_in.close();

	      int audioLength = audio.length;
	      //delay fourth arg
	      int delay = (args.length < 4 ) ? DELAY : Integer.parseInt(args[3]);

	      //buffersize fifth arg
	      int bufSize = (args.length < 5 ) ? BUFFERSIZE : Integer.parseInt(args[4]);
	      
	      //checks for bufSize
	      if(bufSize <0 && bufSize>65535){
	         bufSize = BUFFERSIZE;
		 System.out.println("Program Running with default buffer Size");
                }
		//Sending file 
		DatagramSocket sockid = new DatagramSocket();
		int remaining = audioLength;
		InetAddress inet = InetAddress.getByName(args[0]);
//		System.out.println(args[0]);
		byte[] chunk = new byte[bufSize];
		//System.out.println(remaining);
		while(remaining >= bufSize){
		 for(int i=0;i<bufSize;i++)
		    chunk[i]= audio[audioLength - remaining +i];
		 Thread.sleep(delay);
		 sockid.send(new DatagramPacket(chunk,bufSize,inet,port));
		 remaining -= bufSize;
		 //System.out.println(inet);
		}
		if(remaining >0)
		{
		  chunk = new byte[remaining];
		  for(int i=0;i<remaining;i++)
		     chunk[i]= audio[audioLength - remaining +i];
		  Thread.sleep(delay);
		  sockid.send(new DatagramPacket(chunk,remaining,inet,port));
		// System.out.println("Sending remaining");

		}
		sockid.close();
	  }catch(Exception e){
	    e.printStackTrace();
	  }
      }// main
  }//class

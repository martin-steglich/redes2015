import java.io.*;
import java.net.*;

class UDPServidor{

	public static void main(String args[]) throws Exception{
		//InetAddress group = InetAddress.getByName("225.5.4.29");	
		InetAddress ip = InetAddress.getByName("localhost");	
		DatagramSocket socketServidor = new DatagramSocket(9876, ip);
		//MulticastSocket s = new MulticastSocket(6789);
 		//s.joinGroup(group);

		

		while(true){
			byte[] recibirDatos = new byte[1024];
			byte[] enviarDatos = new byte[1025];
			DatagramPacket recibirPaquete = new DatagramPacket(recibirDatos, recibirDatos.length);
			socketServidor.receive(recibirPaquete);
			
			String mensajeRecibido = new String(recibirPaquete.getData());
			System.out.println(mensajeRecibido);
			enviarDatos = mensajeRecibido.getBytes();
			//InetAddress ipAddress = "localhost";

			int puerto = recibirPaquete.getPort();
			System.out.println("PORT: " + puerto);
			DatagramPacket enviarPaquete = new DatagramPacket(enviarDatos, enviarDatos.length, ip, puerto-1);
			//s.send(enviarPaquete);
			socketServidor.send(enviarPaquete);
		}
	}

}
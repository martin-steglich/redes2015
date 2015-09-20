import java.io.*;
import java.net.*;

class UDPServidor{

	public static void main(String args[]) throws Exception{
		//InetAddress group = InetAddress.getByName("225.5.4.29");	
		InetAddress ip = InetAddress.getByName("localhost");	
		DatagramSocket socketServidor = new DatagramSocket(54321, ip);
		//MulticastSocket s = new MulticastSocket(6789);
 		//s.joinGroup(group);

		

		while(true){
			byte[] recibirDatos = new byte[1024];
			byte[] enviarDatos = new byte[1025];
			DatagramPacket recibirPaquete = new DatagramPacket(recibirDatos, recibirDatos.length);
			socketServidor.receive(recibirPaquete);
			
			String mensajeRecibido = new String(recibirPaquete.getData());
			System.out.println(mensajeRecibido);
			Integer puerto = Integer.parseInt((mensajeRecibido.split(">"))[0]);
			String mensajeAEnviar = (mensajeRecibido.split(">"))[1];
			enviarDatos = mensajeAEnviar.getBytes();
			//InetAddress ipAddress = "localhost";

			//int puerto = recibirPaquete.getPort();
			InetAddress ipCliente = recibirPaquete.getAddress();

			System.out.println("MENSAJE: " + mensajeAEnviar);
			System.out.println("PORT: " + puerto);
			System.out.println("IP: " + ipCliente.toString());
			DatagramPacket enviarPaquete = new DatagramPacket(enviarDatos, enviarDatos.length, ipCliente, puerto+1);
			//s.send(enviarPaquete);
			socketServidor.send(enviarPaquete);
		}
	}

}
// Programa que prueba la funcionalidad del master solicitando
// la longitud de los ficheros recibidos como argumentos

package tests;
import java.util.List;
import java.rmi.*;
import interfaces.*;

public class GetAllocatedChunks {
    static public void main (String args[]) {
        if (args.length!=5) {
            System.err.println("Usage: GetAllocatedChunks RegistryHostname RegistryHostname file nchunk size");
            return;
        }
        if (System.getSecurityManager() == null)
            System.setSecurityManager(new SecurityManager());

        try {
            Master m = (Master) Naming.lookup("//" + args[0] + ":" + args[1] + "/GFS_master");
	    File file = m.lookup(args[2]);
	    List <Chunk> cl = file.getChunkDescriptors(Integer.parseInt(args[3]), Integer.parseInt(args[4]));
	    for (Chunk c : cl) {
                if (c==null)
	    	    System.out.println("chunk no asignado");
		else {
	    	    System.out.println("chunk " + c.getChunkName());
	    	    for (DataNode d : c.getChunkDataNodes())
	    		System.out.println("datanode " + d.getName());
	        }
	    }
        }
        catch (RemoteException e) {
            System.err.println("Error de comunicacion: " + e.toString());
            System.exit(1);
        }
        catch (Exception e) {
            System.err.println("Excepcion en GetAllocatedChunks:");
            e.printStackTrace();
            System.exit(1);
        }
    }
}

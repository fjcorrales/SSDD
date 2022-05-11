// Programa que prueba la funcionalidad del master solicitando
// la longitud de los ficheros recibidos como argumentos

package tests;
import java.util.List;
import java.rmi.*;
import interfaces.*;

public class CheckFilesLength {
    static public void main (String args[]) {
        if (args.length<3) {
            System.err.println("Usage: CheckFilesLength RegistryHostname RegistryHostname file...");
            return;
        }
        if (System.getSecurityManager() == null)
            System.setSecurityManager(new SecurityManager());

        try {
            Master m = (Master) Naming.lookup("//" + args[0] + ":" + args[1] + "/GFS_master");
	    File [] files = new File[args.length-2];
	    for (int i=2; i<args.length; i++) {
	    	System.out.println("lookup fichero 1ª vez " + args[i]);
		files[i-2]=m.lookup(args[i]);
	    }
	    if (args.length>3) {
                if (files[0].equals(files[1])) {
                    System.err.println("Error: lookups != fichero con mismo valor");
                    return;
                }
            }
	    File [] filesBis = new File[args.length-2];
	    for (int i=2; i<args.length; i++) {
	    	System.out.println("lookup fichero 2ª vez " + args[i]);
		filesBis[i-2]=m.lookup(args[i]);
	    }
	    for (int i=2; i<args.length; i++) {
                if (!files[i-2].equals(filesBis[i-2])) {
                    System.err.println("Error: 2º lookup != 1º");
		    return;
	        }
	    }
	    for (int i=2; i<args.length; i++) {
		int l = files[i-2].getNumberOfChunks();
	    	System.out.println("longitud de fichero " + args[i] +
                    " = " + l + " chunks");
	    }
        }
        catch (RemoteException e) {
            System.err.println("Error de comunicacion: " + e.toString());
            System.exit(1);
        }
        catch (Exception e) {
            System.err.println("Excepcion en CheckFilesLength:");
            e.printStackTrace();
            System.exit(1);
        }
    }
}

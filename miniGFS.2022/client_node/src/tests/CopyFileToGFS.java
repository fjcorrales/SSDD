// Ejemplo que copia un fichero local al miniGFS.
// Por las restricciones del miniGFS, el fichero copiado
// tendrá un tamaño múltiplo del chunkSize
package tests;
import java.io.FileInputStream;
import java.io.IOException;
import java.rmi.*;
import client.GFSFile;
class CopyFileToGFS {
    static public void main(String [] args) {
        if (args.length!=2) {
            System.err.println("Usage: CopyFileToGFS file chunkSize");
            return;
        }
        if (System.getSecurityManager() == null)
            System.setSecurityManager(new SecurityManager());
        try {
            int chunkSize = Integer.parseInt(args[1]);
            FileInputStream f1 = new FileInputStream(args[0]);
            GFSFile f2 = new GFSFile(args[0]);
            byte [] buf = new byte[chunkSize];
            while (f1.read(buf)>0)
                f2.write(buf);
        } catch (RemoteException e) {
            System.err.println("error accediendo a fichero destino");
            e.printStackTrace();
        } catch (IOException e) {
            System.err.println("error accediendo a fichero origen");
            e.printStackTrace();
        }
    }
}
        


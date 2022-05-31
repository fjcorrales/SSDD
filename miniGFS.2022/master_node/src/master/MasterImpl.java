// Implementación de la interfaz remota Master

package master;
import java.rmi.*;
import java.rmi.server.*;
import manager.*;
import interfaces.*;
import java.util.*;

public class MasterImpl extends UnicastRemoteObject implements Master {
    public static final long  serialVersionUID=1234567891;

	private Map<String,FileImpl> mapa;
	private ManagerImpl manager;
	private int rep;
	
    public MasterImpl(ManagerImpl m, int replica) throws RemoteException {
    	mapa = new HashMap<String, FileImpl>();
    	manager = m;
    	rep = replica;
    }
    // obtiene acceso a la metainformación de un fichero
    public synchronized File lookup(String fname) throws RemoteException {
        if(!mapa.containsKey(fname)){
        	FileImpl nuevo = new FileImpl(manager, rep);
        	mapa.put(fname,nuevo);
        }
        return mapa.get(fname);
    }
}

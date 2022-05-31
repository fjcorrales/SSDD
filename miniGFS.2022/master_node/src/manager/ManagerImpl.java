// Implementación de la interfaz del manager

package manager;
import java.util.List;
import java.util.ArrayList;
import java.rmi.*;
import java.rmi.server.*;

import interfaces.*;

public class ManagerImpl extends UnicastRemoteObject implements Manager {
    public static final long  serialVersionUID=1234567891;

	private ArrayList<DataNode> allDataNodes;
	private int lastDataNode;

    public ManagerImpl() throws RemoteException {
    	allDataNodes = new ArrayList<>();
    	lastDataNode = 0;
    }
    // alta de un nodo de datos
    public synchronized void addDataNode(DataNode n) throws RemoteException {
    	allDataNodes.add(n);
    }
    // obtiene lista de nodos de datos del sistema
    public synchronized List <DataNode> getDataNodes() throws RemoteException {
        return allDataNodes;
    }
    // método no remoto que selecciona un nodo de datos para ser usado
    // para almacenar un chunk
    public synchronized DataNode selectDataNode() {
    	DataNode result;
    	if(lastDataNode < allDataNodes.size()){
    		result = allDataNodes.get(lastDataNode);
    	} else {
    		lastDataNode = 0;
    		result = allDataNodes.get(lastDataNode);
    	}
    	lastDataNode++;
        return result;
    }
}

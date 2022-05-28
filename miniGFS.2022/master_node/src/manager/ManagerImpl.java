// Implementación de la interfaz del manager

package manager;
import java.util.List;
import java.util.ArrayList;
import java.rmi.*;
import java.rmi.server.*;

import interfaces.*;

public class ManagerImpl extends UnicastRemoteObject implements Manager {
    public static final long  serialVersionUID=1234567891;

    public ManagerImpl() throws RemoteException {
    }
    // alta de un nodo de datos
    public synchronized void addDataNode(DataNode n) throws RemoteException {
    }
    // obtiene lista de nodos de datos del sistema
    public synchronized List <DataNode> getDataNodes() throws RemoteException {
        return null;
    }
    // método no remoto que selecciona un nodo de datos para ser usado
    // para almacenar un chunk
    public synchronized DataNode selectDataNode() {
        return null;
    }
}

// Interfaz remota Manager
// La usan los nodos de datos para darse de alta en el sistema
// MO MODIFICAR

package interfaces;
import java.rmi.*;
import java.util.List;

public interface Manager extends Remote {
    // alta de un nodo de datos
    public void addDataNode(DataNode n) throws RemoteException;
    // obtiene lista de nodos de datos del sistema
    public List <DataNode> getDataNodes() throws RemoteException;
}

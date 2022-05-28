// Cliente para probar el servicio.
package tests;
import java.util.*;
import java.rmi.*;
import client.GFSFile;

class Test {
    static HashMap<Integer, GFSFile> descriptorMap = new HashMap<Integer, GFSFile>();
    static char car='A';
    static int desp=0;
    static int desc=0;
    static int chunkSize=1;

    static private boolean doOpen(Scanner ent) throws RemoteException {
        System.err.println("Introduzca nombre de fichero: ");
        if (!ent.hasNextLine()) return false;
        String lin = ent.nextLine();
        Scanner s = new Scanner(lin);
        if (!s.hasNext()) return false;
        String fich = s.next();
        GFSFile f = new GFSFile(fich);
        descriptorMap.put(desc, f);
        System.err.println("Fichero abierto ID " + desc++);
        return true;
    }

    static private boolean doRead(Scanner ent) throws RemoteException {
        boolean res = false;
        System.err.println("Introduzca ID de fichero y cantidad a leer (debe ser múltiplo de chunkSize)");
        if (!ent.hasNextLine()) return false;
        String lin = ent.nextLine();
        Scanner s = new Scanner(lin);
        if (s.hasNextInt()) {
            int ID = s.nextInt();
            if (s.hasNextInt()) {
                int tam = s.nextInt();
                res = true;
                GFSFile f = descriptorMap.get(ID);
                if (f ==null)
                    System.err.println("ID de fichero inválido");
		else if (tam%chunkSize != 0)
                    System.err.println("tamaño no es múltiplo de chunkSize");
                else {
                    byte [] buf = new byte [tam];
                    int leido= f.read(buf);
                    System.err.println("Leidos " + leido + " bytes");
                    if (leido>0) {
                        System.out.write(buf, 0, leido);
                        System.err.println();
                    }
                }
           }
        }
        return res;
    }

    static private boolean doWrite(Scanner ent) throws RemoteException {
        boolean res = false;
        System.err.println("Introduzca ID de fichero y cantidad a escribir (debe ser múltiplo de chunkSize)");
        if (!ent.hasNextLine()) return false;
        String lin = ent.nextLine();
        Scanner s = new Scanner(lin);
        if (s.hasNextInt()) {
            int ID = s.nextInt();
            if (s.hasNextInt()) {
                int tam = s.nextInt();
                res = true;
                GFSFile f = descriptorMap.get(ID);
                if (f ==null)
                    System.err.println("ID de fichero inválido");
                else if (tam%chunkSize != 0)
                    System.err.println("tamaño no es múltiplo de chunkSize");
                else {
                    byte [] buf = new byte[tam];
                    for (int i=0; i<tam; i++, desp=(desp+1)%26) buf[i] = (byte)(car + desp);
                    f.write(buf);
                    System.err.println("Escritos " +tam+ " bytes");
                }
           }
        }
        return res;
    }
    static private boolean doSeek(Scanner ent) throws RemoteException {
        boolean res = false;
        System.err.println("Introduzca ID de fichero y nueva posición (debe ser múltiplo de chunkSize)");
        if (!ent.hasNextLine()) return false;
        String lin = ent.nextLine();
        Scanner s = new Scanner(lin);
        if (s.hasNextInt()) {
            int ID = s.nextInt();
            if (s.hasNextInt()) {
                int pos = s.nextInt();
                res = true;
                GFSFile f = descriptorMap.get(ID);
                if (f ==null)
                    System.err.println("ID de fichero inválido");
                else if (pos%chunkSize != 0)
                    System.err.println("offset no es múltiplo de chunkSize");
                else {
                    f.seek(pos);
                    System.err.println("Puntero colocado en posicion " + pos);
                }
           }
        }
        return res;
    }

    static private boolean doGetFilePointer(Scanner ent) throws RemoteException {
        boolean res = false;
        System.err.println("Introduzca ID de fichero");
        if (!ent.hasNextLine()) return false;
        String lin = ent.nextLine();
        Scanner s = new Scanner(lin);
        if (s.hasNextInt()) {
            int ID = s.nextInt();
            res = true;
            GFSFile f = descriptorMap.get(ID);
            if (f ==null)
                System.err.println("ID de fichero inválido");
            else {
                int pos = f.getFilePointer();
                System.err.println("Puntero colocado en posicion " + pos);
           }
        }
        return res;
    }

    static private boolean doLength(Scanner ent) throws RemoteException {
        boolean res = false;
        System.err.println("Introduzca ID de fichero");
        if (!ent.hasNextLine()) return false;
        String lin = ent.nextLine();
        Scanner s = new Scanner(lin);
        if (s.hasNextInt()) {
            int ID = s.nextInt();
            res = true;
            GFSFile f = descriptorMap.get(ID);
            if (f ==null)
                System.err.println("ID de fichero inválido");
            else {
                int tam = f.length();
                System.err.println("tamaño del fichero " + tam);
           }
        }
        return res;
    }

    static private void prompt() {
        System.err.println("Seleccione operacion");
        System.err.println("\t0 open|1 read|2 write|3 seek|4 getFilePointer|5 length (Ctrl-D para terminar)");
    }

    static public void main(String args[]) {
        if (System.getSecurityManager() == null)
            System.setSecurityManager(new SecurityManager());

        String cSize=System.getenv("CHUNKSIZE");
        if (cSize != null) 
            chunkSize=Integer.parseInt(cSize);
        try {
            while (true) {
                boolean formatoOK = false;
                Scanner ent = new Scanner(System.in);
                prompt();
                if (!ent.hasNextLine()) return;
                String lin = ent.nextLine();
                Scanner s = new Scanner(lin);
                if (s.hasNextInt()) {
                   int op = s.nextInt();
                   switch (op) {
                       case 0: formatoOK = doOpen(ent); break;
                       case 1: formatoOK = doRead(ent); break;
                       case 2: formatoOK = doWrite(ent); break;
                       case 3: formatoOK = doSeek(ent); break;
                       case 4: formatoOK = doGetFilePointer(ent); break;
                       case 5: formatoOK = doLength(ent); break;
                    }
                }
                if (!formatoOK)
                     System.err.println("Error en formato de operacion");
            }
        }
        catch (Exception e) {
            System.err.println("Excepcion en Test:");
            e.printStackTrace();
        }
        finally {
            System.exit(0);
        }

    }
}

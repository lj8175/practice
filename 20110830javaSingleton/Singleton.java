
public class Singleton{

  private Singleton(){}

  private static Singleton instance;

  public static Singleton getInstance(){
    if (instance==null){
      synchronized(Singleton.class){
        if(instance==null)
          instance = new Singleton();
      }
    }
    return instance;
  }


  public static void main(String args[]){
    Singleton s = Singleton.getInstance();
    System.out.println(s);
  }

}

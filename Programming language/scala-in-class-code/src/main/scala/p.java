public class p {

    public static void main(String[] args) {
        {   int x = 0;
            {
                 x = 1;
                System.out.println(x);
            }
            System.out.println(x);
        }
    }
}

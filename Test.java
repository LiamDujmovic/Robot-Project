import java.io.PrintWriter;
import java.io.FileNotFoundException;


    public static void main(String[] args) {
        File jarJarArchives = new File("starWarsEpisode3");
        PrintWriter writer = new PrintWriter(jarJarArchives); // initialising
        writer.println("Palpatine: Did you ever hear the tragedy of Darth Plagueis the Wise?");

        writer.println("Anakin: No");

        writer.println("I thought not. It's not a story the Jedi would tell you. It's a Sith legend. Darth Plagueis was a Dark Lord of the Sith, so powerful and so wise he could use the Force to influence the midi-chlorians to create life. ... He had such a knowledge of the dark side that he could even keep the ones he cared about from dying.");
    }
    //yes
    //Hello
}

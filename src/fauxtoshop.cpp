#include <iostream>
#include <random>
#include"helperfns.h"

static void option1(const Grid<int> &original,GBufferedImage &img1);
static void option2(const Grid<int> &original,GBufferedImage &img1);
static void option3( Grid<int> &backg,GBufferedImage &img1);
static void option4();

int main() {

    GWindow gw; //Console window instance
    gw.setTitle("Fauxtoshop");
    gw.setVisible(true);

    GBufferedImage img; //create a 2D pixel image
    string filename; //user input filename string
    int userInt;
    string userSave; //file name of image to be saved
    Grid<int>original; //maps the 2D pixels into int(RGB) grid scheme

    cout << "Welcome to Fauxtoshop!" << endl;

    //ask the user for an image
    do {
        cout << "Enter name of file to open (or blank to quit):" << flush;

        getline(cin,filename); //convert input console to filename string
        if (filename.length()==0) {
            gw.setVisible(false);
            gw.setExitOnClose();
            //exit the program if the input is 0
            return 0;
        }
    } while (!openImageFromFilename(img, filename));

    //once the filename is loaded to img assign it to Grid<int> 2D matrix
    // each element of the grid represents RGB (int) values of the image
    //passed the img by reference (can also be returned)
    img.toGrid(original);

    cout << "Opening image file, may take a minute.." <<endl;
    //set the window console to the same size as the image
    gw.setSize(img.getWidth(), img.getHeight());
    //add the selected imgage to the console notice we are passing a pointer bc the function takes a pointer as a parameter
    gw.add(&img,0,0);

    cout << "Which image fileter would you like to apply?" <<endl;
    cout << "               1.Scatter" <<endl;
    cout << "               2.Edge detection" <<endl;
    cout << "               3.\"Green screen\" with another image" <<endl;
    cout << "               4.Compare image with another image" <<endl;

    do {
        cout << "Your choice: "<<flush;
        userInt=getInteger();
    } while (!(userInt>0 && userInt<5));



    if (userInt==1) {
        option1(original,img);
    }

    else if (userInt==2) {

        option2(original,img);
    }

    else if (userInt==3) {
        option3(original,img);
    }
    else {
        option4();

    }

    cout<<"Enter filename to save image (or blank to skip saving): "<<flush;
    userSave=getLine("");
    if (userSave.size()!=0) {
        cout<<saveImageToFilename(img,userSave)<<endl;
    }
    else {
        return 0;
    }



//    int row, col;
//    getMouseClickLocation(row, col);
    return 0;
}



//pass by value
static void option1(const Grid<int> &original,GBufferedImage &img1){

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

    int degree=0;
    int tempPix;
    int rowRan=0;
    int colRan=0;


    do {
        cout<<"Enter a degree of scatter [1-100]"<<flush;
        degree=getInteger();
    } while (!(degree>=0&&degree<=100));

    // create an object of type Grid named "other" and call its constructor
    Grid<int> other (original.numRows(),original.numCols()) ;

    for (int i=0;i<other.numRows();i++) {
        for (int j=0;j<other.numCols();j++) {

            //randomnly genereate a int that is within the specified range from the current
            //row and column
            uniform_int_distribution<> rowe(i-degree, i+degree);
            uniform_int_distribution<> cole(j-degree, j+degree);

            //check that the selected row and col are within bounds
            do {
                //generate random row and col
                rowRan=rowe(gen);
                colRan=cole(gen);

            } while (!(original.inBounds(rowRan,colRan)));
            tempPix=original.get(rowRan,colRan);
            other[i][j]=tempPix;

        }

    }

    //change the current console img to the genereated int OTHER
    img1.fromGrid(other);


}

//Helper Function that gets the RGB color difference of two pixels
static int maxDiff(int pixel1, int pixel2){

    int r1,g1,b1;
    int r2,g2,b2;

    //parameters are passed by reference thus we can change three variable
    GBufferedImage::getRedGreenBlue(pixel1,r1,g1,b1);
    GBufferedImage::getRedGreenBlue(pixel2,r2,g2,b2);
    //see handout must calculate magnituded
    int rMax=abs(r1-r2);
    int gMax=abs(g1-g2);
    int bMax=abs(b1-b2);

    return max(max(rMax,bMax),gMax);
}


static void option2(const Grid<int> &original,GBufferedImage &img1){
    int threshold=18;
    // create an object named "other" and call its constructor
    Grid<int> other (original.numRows(),original.numCols()) ;

    for (int i=0;i<other.numRows();i++) {
        for (int j=0;j<other.numCols();j++) {

            //by default it is white
            other[i][j]=WHITE;

            for (int r=i-1;r<=i+1;r++) {
                for (int c=j-1;c<=j+1;c++) {

                    //if this is true than it is an edge so change the pixel from white to black
                    if (original.inBounds(r,c)&& maxDiff(original.get(i,j),original.get(r,c))>threshold){
                        other[i][j]=BLACK;
                        //break out of nested for loop
                        r=i+2;
                        c=j+2;


                    }


                }

            }

        }

    }

    img1.fromGrid(other);

}


static void option3( Grid<int> &backg,GBufferedImage &img1){
    int threshold=60;
    GBufferedImage stick;
    string filename= "cat-green.jpg";
    openImageFromFilename(stick, filename);
    //see option 1 for details same process
    Grid<int> stickGr;
    stick.toGrid(stickGr);

    //client defined
//    int row=300;
//    int col=50;

     int row, col;
    getMouseClickLocation(row, col);
    cout<<row<<col<<endl;
    cout<<"rows are"<<stickGr.numRows()<<endl;
    cout<<"cols are"<<stickGr.numCols()<<endl;

    for (int i=row;i<backg.numRows();i++) {
        for (int j=col;j<backg.numCols();j++) {

            if (stickGr.inBounds(i-row,j-col)&& maxDiff(stickGr.get(i-row,j-col),GREEN)>threshold){
                backg[i][j]=stickGr[i-row][j-col];

            }

        }

    }

    img1.fromGrid(backg);
}

static void option4(){

    GBufferedImage mine;
    string filename= "cgreen.jpg";
    openImageFromFilename(mine, filename);

    GBufferedImage teach;
    string filename2= "cgreen.jpg";
    openImageFromFilename(teach, filename2);

    cout<<mine.countDiffPixels(teach)<<endl;

}







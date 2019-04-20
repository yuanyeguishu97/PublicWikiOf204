/* Swc2Img_plugin.cpp
 * This is a test plugin, you can use it as a demo.
 * 2019-4-20 : by yufuhao
 */
 
#include "v3d_message.h"
#include <vector>
#include <iostream>
#include "Swc2Img_plugin.h"
using namespace std;
Q_EXPORT_PLUGIN2(Swc2Img, TestPlugin);
 
QStringList TestPlugin::menulist() const
{
	return QStringList() 
        <<tr("swc2img")
		<<tr("about");
}

QStringList TestPlugin::funclist() const
{
	return QStringList()
		<<tr("func1")
		<<tr("func2")
		<<tr("help");
}

void SetImg(unsigned char * img,NeuronSWC point_a,V3DLONG sz[],int id=255)
{
    V3DLONG ans;
    NeuronSWC p1;
    p1.x=V3DLONG(point_a.x+0.5);
    p1.y=V3DLONG(point_a.y+0.5);
    p1.z=V3DLONG(point_a.z+0.5);
    ans=(p1.z-1)*sz[0]*sz[1]+(p1.y-1)*sz[0]+(p1.x-1);
    if(p1.x<0||p1.y<0||p1.z<0||p1.x>sz[0]||p1.y>sz[01]||p1.z>sz[2]||ans>sz[0]*sz[1]*sz[2]||ans<0)
    {
        cout<<"input a wrong interger_point ans:"<<ans<<endl;
        cout<<"input x "<<p1.x<<" y "<<p1.y<<" z "<<p1.z<<""<<endl;
        return ;
    }
    img[ans]=id;
//    cout<<"point_a.x "<<point_a.x<<" point_a.y "<<point_a.y<<" point_a.z "<<point_a.z<<endl;
}

vector<NeuronSWC> pointsOfTwoPoint(NeuronSWC point_a,NeuronSWC point_b)
{
    vector<NeuronSWC> pp;
    pp.push_back(point_a);
    if(point_a==point_b)
    {
        return pp;
    }
    NeuronSWC direction;
    direction.x=(point_b.x-point_a.x);
    direction.y=(point_b.y-point_a.y);
    direction.z=(point_b.z-point_a.z);
    float length=sqrt(direction.x*direction.x+direction.y*direction.y+direction.z*direction.z);
    if(length==0)
    {
        cout<<"something wrong length==0 pointsOfTwoPoint"<<endl;
        return pp;
    }
    direction.x/=(length*2);
    direction.y/=(length*2);
    direction.z/=(length*2);
    for(int i=0;i<length;i++)
    {
        point_a.x+=direction.x;
        point_a.y+=direction.y;
        point_a.z+=direction.z;
        pp.push_back(point_a);
    }
    return pp;
}


//v1  need sz
unsigned char * NeuronTree2img(NeuronTree nt1,V3DLONG sz[])
{
    //init
    unsigned char * swc_img;
    try{swc_img=new unsigned char [sz[0]*sz[1]*sz[2]];}
    catch(...) {v3d_msg("cannot allocate memory for swc_img."); return NULL;}
    for(V3DLONG i=0;i<sz[0]*sz[1]*sz[2];i++){swc_img[i]=0;}
//    cout<<"init is over"<<endl;

    for(V3DLONG i=0;i<nt1.listNeuron.size();i++)
    {
        NeuronSWC  S1,S2;
        S1=nt1.listNeuron.at(i);
        SetImg(swc_img,S1,sz);
        if(S1.parent<=-1)
        { continue;
        }
//        cout<<"current S1 i "<<i<<" x "<<S1.x<<" y "<<S1.y<<" z "<<S1.z<<endl;
        S2=nt1.listNeuron.at(nt1.hashNeuron.value(S1.parent));
//        cout<<"current S2 x "<<S2.x<<" y "<<S2.y<<" z "<<S2.z<<endl;
        vector<NeuronSWC> needtoshow;
        needtoshow=pointsOfTwoPoint(S1,S2);
        for(int j=0;j<needtoshow.size();j++)
        {
            SetImg(swc_img,needtoshow.at(j),sz);
        }
//        cout<<"one point is over"<<endl;
    }
    return swc_img;
}

void TestPlugin::domenu(const QString &menu_name, V3DPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("swc2img"))
    {
        QList <V3dR_MainWindow *> list_3dviewer = callback.getListAll3DViewers();
        if (list_3dviewer.size() < 1)
        {
            v3d_msg("Please open  a SWC file from the main menu first! list_3dviewer");
            return ;
        }
        V3dR_MainWindow *surface_win = list_3dviewer[0];
        if (!surface_win){
            v3d_msg("Please open up a SWC file from the main menu first!");
            return ;
        }
        QList<NeuronTree> * mTreeList = callback.getHandleNeuronTrees_Any3DViewer(surface_win);
        if(mTreeList->size()<=0)
        {
            return ;
        }
        NeuronTree filetree=mTreeList->at(mTreeList->size()-1);
//        NeuronTree filetree=callback.getSWC(callback.currentImageWindow());
        cout<<"input filetree size is "<<filetree.listNeuron.size()<<endl;



        Image4DSimple* img1=callback.getImage(callback.currentImageWindow());
        V3DLONG sz[4];
        sz[0]=img1->getXDim();
        sz[1]=img1->getYDim();
        sz[2]=img1->getZDim();
        sz[3]=1;

        unsigned char * img_swc=NeuronTree2img(filetree,sz);
        if(img_swc==NULL)
        {
            cout<<"img_swc==NULL"<<endl;
            return;
        }
        QString filename=QFileDialog::getSaveFileName(0, QObject::tr("Save File"),
                                         "",
                                         QObject::tr("Tif file (*.tif)"
                                             ";;Tiff file 	(*.tiff)"
                                             ));
        if(filename.isEmpty())
        {
            return;
        }

        simple_saveimage_wrapper(callback,filename.toUtf8().data(),img_swc,sz,1);


    }
	else
	{
		v3d_msg(tr("This is a test plugin, you can use it as a demo.. "
			"Developed by yufuhao, 2019-4-20"));
	}
}

bool TestPlugin::dofunc(const QString & func_name, const V3DPluginArgList & input, V3DPluginArgList & output, V3DPluginCallback2 & callback,  QWidget * parent)
{
	vector<char*> infiles, inparas, outfiles;
	if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
	if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
	if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

	if (func_name == tr("func1"))
	{
		v3d_msg("To be implemented.");
	}
	else if (func_name == tr("func2"))
	{
		v3d_msg("To be implemented.");
	}
	else if (func_name == tr("help"))
	{
		v3d_msg("To be implemented.");
	}
	else return false;

	return true;
}


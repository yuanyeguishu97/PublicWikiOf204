/* merge_same_point_plugin.cpp
 * This is a test plugin, you can use it as a demo.
 * 2019-4-22 : by YourName
 */
 
#include "v3d_message.h"
#include <vector>
#include "merge_same_point_plugin.h"
#include <iostream>
using namespace std;
Q_EXPORT_PLUGIN2(merge_same_point, TestPlugin);
 
QStringList TestPlugin::menulist() const
{
	return QStringList() 
        <<tr("merge_same_point_-1")
		<<tr("about");
}

QStringList TestPlugin::funclist() const
{
	return QStringList()
		<<tr("func1")
		<<tr("func2")
		<<tr("help");
}

void TestPlugin::domenu(const QString &menu_name, V3DPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("merge_same_point_-1"))
    {
        //get neuron tree
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

//        Image4DSimple* img1=callback.getImage(callback.currentImageWindow());
//        V3DLONG sz[4];
//        sz[0]=img1->getXDim();
//        sz[1]=img1->getYDim();
//        sz[2]=img1->getZDim();
//        sz[3]=1;

        vector<MyMarker*> temp_neuron=NeuronTree2vectorofMyMarker(filetree);
        V3DLONG neuron_num=filetree.listNeuron.size();
        char *flag;
        try {
            flag=new char[neuron_num];
        } catch (...) {
            cout<<"flag=new char[filetree.listNeuron.size()]; error"<<endl;
            return ;
        }
        for(V3DLONG i=0;i<neuron_num;i++) {flag[i]=1;}
        cout<<"init is over"<<endl;

        V3DLONG count=0;
        for(V3DLONG i=0;i<neuron_num;i++)
        {
            //parent’ parent equal -1
            MyMarker* cur_marker=temp_neuron[i];
            MyMarker* parent=cur_marker->parent;

            if(parent!=NULL&&parent->parent==NULL)
            {
                for(V3DLONG j=0;j<neuron_num;j++)
                {
                    MyMarker* temp_1=temp_neuron[j];
                    if(temp_1!=parent&&parent->x==temp_1->x&&parent->y==temp_1->y&&parent->z==temp_1->z)
                    {
                        if(temp_1->parent!=NULL)
                        {
                            cur_marker->parent=temp_1;
                            flag[filetree.hashNeuron.value(filetree.listNeuron.at(i).parent)]=0;
                            break;
                        }
                        else
                        {
                            if(i<j)
                            {
                                cur_marker->parent=temp_1;
                                flag[filetree.hashNeuron.value(filetree.listNeuron.at(i).parent)]=0;
                            }

                        }
                    }
                }
            }
        }

        vector<MyMarker*> after_swc;
        for (int j=0;j<neuron_num;j++)
        {
            if(flag[j] == 1)
            {
                after_swc.push_back(temp_neuron[j]);
            }
       }
        if(flag) {delete[] flag; flag = 0;}
        NeuronTree final_tree=vectorofMyMarker2NeuronTree(after_swc);
        for(V3DLONG i=0;i<temp_neuron.size();i++)
        {
            delete temp_neuron.at(i);
        }

        QString filename=QFileDialog::getSaveFileName(0, QObject::tr("Save File"),
                                         "",
                                         QObject::tr("swc file (*.swc)"
                                             ));
        if(filename.isEmpty())
        {
            return;
        }
        writeSWC_file(filename,final_tree);


    }
	else
	{
		v3d_msg(tr("This is a test plugin, you can use it as a demo.. "
			"Developed by YourName, 2019-4-22"));
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


#include"neuron_tools.h"
QList <ImageMarker> LandmarkListQList_ImageMarker(LandmarkList marker_l)
{
    QList <ImageMarker> markers;
    for(V3DLONG i=0;i<marker_l.size();i++)
    {
        ImageMarker temp;
        temp.x=marker_l.at(i).x;
        temp.y=marker_l.at(i).y;
        temp.z=marker_l.at(i).z;
        temp.z=marker_l.at(i).z;
        temp.radius=marker_l.at(i).radius;
        temp.shape=marker_l.at(i).shape;
        temp.name=QString::fromStdString(marker_l.at(i).name);
        temp.color.r=marker_l.at(i).color.r;
        temp.color.g=marker_l.at(i).color.g;
        temp.color.b=marker_l.at(i).color.b;
        markers.push_back(temp);
    }
    return markers;


}
LandmarkList QList_ImageMarker2LandmarkList(QList <ImageMarker> markers)
{
    LandmarkList marker_l;
    for(V3DLONG i=0;i<markers.size();i++)
    {
        LocationSimple temp;
        temp.x=markers.at(i).x;
        temp.y=markers.at(i).y;
        temp.z=markers.at(i).z;
        temp.z=markers.at(i).z;
        temp.radius=markers.at(i).radius;
        temp.shape=PxLocationMarkerShape(markers.at(i).shape);
        temp.name=markers.at(i).name.toStdString();
        temp.color.r=markers.at(i).color.r;
        temp.color.g=markers.at(i).color.g;
        temp.color.b=markers.at(i).color.b;
        marker_l.push_back(temp);
    }
    return marker_l;

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
    cout<<"point_a.x "<<point_a.x<<" point_a.y "<<point_a.y<<" point_a.z "<<point_a.z<<endl;
    cout<<"interger_point is ok: "<<ans<<endl;



}

//NeuronSWC interger(NeuronSWC point_a)
//{
//    NeuronSWC p1;

//    p1.x=V3DLONG(point_a.x+0.5);
//    p1.y=V3DLONG(point_a.y+0.5);
//    p1.z=V3DLONG(point_a.z+0.5);
//    return p1;

//}
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
    cout<<"init is over"<<endl;

    for(V3DLONG i=0;i<nt1.listNeuron.size();i++)
    {
        cout<<"begin";
        NeuronSWC  S1,S2;
        S1=nt1.listNeuron.at(i);
        SetImg(swc_img,S1,sz);
        cout<<"parent is "<<S1.parent;
        if(S1.parent<=-1)
        { continue;
        }//else if S1.parent don't exist
        cout<<"current S1 i "<<i<<" x "<<S1.x<<" y "<<S1.y<<" z "<<S1.z<<endl;
        S2=nt1.listNeuron.at(nt1.hashNeuron.value(S1.parent));
        cout<<"current S2 x "<<S2.x<<" y "<<S2.y<<" z "<<S2.z<<endl;
        vector<NeuronSWC> needtoshow;
        needtoshow=pointsOfTwoPoint(S1,S2);
        cout<<"begin1"<<endl;
        for(int j=0;j<needtoshow.size();j++)
        {
            SetImg(swc_img,needtoshow.at(j),sz);
        }
        cout<<"begin1 is over"<<endl;
        cout<<"one point is over"<<endl;
    }


    return swc_img;
}




LocationSimple MyMarker2LocationSimple(MyMarker* marker)
{
    RGBA8 red;
    red.r=255;
    red.g=0;
    red.b=0;
   LocationSimple s;
   s.x=marker->x;
   s.y=marker->y;
   s.z=marker->z;
   s.radius=marker->radius;
   s.color = red;
   return s;
}
//LandmarkList FromNeurontreeGetLeaf(NeuronTree nt)
//{
//    LocationSimple s;
//    RGBA8 red;
//    red.r=255;
//    red.g=0;
//    red.b=0;
////    RGBA8 green;
////    green.r=0;
////    green.g=255;
////    green.b=0;
//    V3DLONG size = nt.listNeuron.size();
//    cout<<"nt.listNeuron.size() "<<size<<endl;
//    QList<NeuronSWC> neuron = nt.listNeuron;
//    int *countOfNeuron;
//    try{countOfNeuron=new int [size];}
//    catch(...) {v3d_msg("cannot allocate memory for countOfNeuron."); return ;}
//    for (V3DLONG i=0;i<size;i++)
//    {
//       countOfNeuron[i]=0;
//    }
//    for (V3DLONG i=0;i<size;i++)
//    {
//        int ind=neuron.at(i).parent;
//        if(ind-1>=0&&ind-1<size)
//        {countOfNeuron[ind-1]++;}
//        else if(ind==-1)
//        {
//            countOfNeuron[i]--;
//        }
//    }
//    LandmarkList swc_tip_list;
////    LandmarkList swc_branch_list;
//    for(V3DLONG i=0;i<size;i++)
//    {
//        if(countOfNeuron[i]==0)
//        {
//            s.x=neuron.at(i).x;
//            s.y=neuron.at(i).y;
//            s.z=neuron.at(i).z;
//            s.radius=neuron.at(i).r;
//            s.color = red;
//            swc_tip_list<<s;
//            //tip
//        }
////        else if(countOfNeuron[i]>=2)
////        {
////            s.x=neuron.at(i).x;
////            s.y=neuron.at(i).y;
////            s.z=neuron.at(i).z;
////            s.radius=neuron.at(i).r;
////            s.color = green;
////            swc_branch_list<<s;
////            //branch
////        }
//    }
//    return swc_tip_list;
//}

MyMarker* LocationSimple2MyMarker(LocationSimple s)
{
    MyMarker* marker=new MyMarker;
    marker->x=s.x;
    marker->y=s.y;
    marker->z=s.z;
    marker->radius=s.radius;
    return marker;
}
vector<MyMarker*> readSWC_file1(string swc_file)
{
    vector<MyMarker*> swc;
    ifstream ifs(swc_file.c_str());
    if(ifs.fail())
    {
        cout<<"open swc file : "<< swc_file <<" error"<<endl;
        return swc;
    }
    map<int, MyMarker*> marker_map;
    map<MyMarker*, int> parid_map;
    while(ifs.good())
    {
        if(ifs.peek() == '#'){ifs.ignore(1000,'\n'); continue;}
        MyMarker *  marker = new MyMarker;
        int my_id = -1 ; ifs >> my_id;
        if(my_id == -1) break;
        if(marker_map.find(my_id) != marker_map.end())
        {
            cerr<<"Duplicate Node. This is a graph file. Please read is as a graph."<<endl; return vector<MyMarker*>();
        }
        marker_map[my_id] = marker;

        ifs>> marker->type;
        ifs>> marker->x;
        ifs>> marker->y;
        ifs>> marker->z;
        ifs>> marker->radius;
        int par_id = -1; ifs >> par_id;

        parid_map[marker] = par_id;
        swc.push_back(marker);
    }
    ifs.close();
    vector<MyMarker*>::iterator it = swc.begin();
    while(it != swc.end())
    {
        MyMarker * marker = *it;
        marker->parent = marker_map[parid_map[marker]];
        it++;
    }
    return swc;
}

bool writeSWC_file1(const QString& filename, const NeuronTree& nt)
{
    QString curFile = filename;
    if (curFile.trimmed().isEmpty()) //then open a file dialog to choose file
    {
        curFile = QFileDialog::getSaveFileName(0,
                                               "Select a SWC file to save the neuronal or relational data... ",
                                               ".swc",
                                               QObject::tr("Neuron structure file (*.swc);;(*.*)"
                                                           ));
        if (curFile.isEmpty()) //note that I used isEmpty() instead of isNull
            return false;
    }

    FILE * fp = fopen(curFile.toLatin1(), "wt");
    if (!fp)
    {
        return false;
    }
    fprintf(fp, "#name %s\n", qPrintable(nt.name.trimmed()));
    fprintf(fp, "#comment %s\n", qPrintable(nt.comment.trimmed()));
    fprintf(fp, "##n,type,x,y,z,radius,parent\n");
    NeuronSWC * p_pt=0;
    for (int i=0;i<nt.listNeuron.size(); i++)
    {
        p_pt = (NeuronSWC *)(&(nt.listNeuron.at(i)));
        fprintf(fp, "%ld %d %6.2f %6.2f %6.2f %f %ld\n",
                p_pt->n, p_pt->type, p_pt->x, p_pt->y, p_pt->z, p_pt->r, p_pt->pn);
    }

    fclose(fp);
    return true;
}
bool saveSWC_file1(string swc_file, vector<MyMarker*> & outmarkers)
{
    if(swc_file.find_last_of(".dot") == swc_file.size() - 1) return saveDot_file(swc_file, outmarkers);
    map<MyMarker*, int> ind;
    ofstream ofs(swc_file.c_str());
    if(ofs.fail())
    {
        cout<<"open swc file error"<<endl;
        return false;
    }
    ofs<<"#name "<<swc_file<<endl;
    ofs<<"##n,type,x,y,z,radius,parent"<<endl;
    for(int i = 0; i < outmarkers.size(); i++) ind[outmarkers[i]] = i+1;

    for(int i = 0; i < outmarkers.size(); i++)
    {
        MyMarker * marker = outmarkers[i];
        int parent_id;
        if(marker->parent == 0) parent_id = -1;
        else parent_id = ind[marker->parent];
        ofs<<i+1<<" "<<marker->type<<" "<<marker->x<<" "<<marker->y<<" "<<marker->z<<" "<<marker->radius<<" "<<parent_id<<endl;
    }
    ofs.close();
    return true;
}

vector<MyMarker*>  NeuronTree2vectorofMyMarker(NeuronTree nt)
{
//    //copy NeuronTree to vector<MyMarker*>
    vector<MyMarker*> swc;

//    map<int, MyMarker*> marker_map;
//    map<MyMarker*, int> parid_map;
//    for(V3DLONG i=0;i<nt.listNeuron.size();i++)
//    {
//        MyMarker *  marker = new MyMarker;
//        marker_map[i] = marker;

//        marker->type=nt.listNeuron.at(i).type;
//        marker->x =nt.listNeuron.at(i).x;
//        marker->y=nt.listNeuron.at(i).y;
//        marker->z=nt.listNeuron.at(i).z;
//        marker->radius=nt.listNeuron.at(i).radius;
////        marker->parent=nt.listNeuron.at(i).parent;
//        parid_map[marker] = nt.listNeuron.at(i).parent;
//        swc.push_back(marker);
//    }

//    vector<MyMarker*>::iterator it = swc.begin();
//    while(it != swc.end())
//    {
//        MyMarker * marker = *it;
//        marker->parent = marker_map[parid_map[marker]];
//        it++;
//    }
    QString tempname="tempname_for_transfer_1.swc";
    // 只保留到小数点后两位
    writeSWC_file1(tempname,nt);
    swc=readSWC_file1(tempname.toStdString());

    return swc;

}

NeuronTree merge_two_neuron(NeuronTree swc1 ,NeuronTree nt)
{
    cout<<"begin to merge two neuron";
    vector<MyMarker*> tree_new=NeuronTree2vectorofMyMarker(nt);
    vector<MyMarker*>  swc=NeuronTree2vectorofMyMarker(swc1);
    for(V3DLONG i=0;i<swc.size();i++)
    {
        MyMarker* temp_point_swc=swc.at(i);
        for(V3DLONG j=0;j<tree_new.size();j++)
        {
            MyMarker* temp_point_nt=tree_new.at(j);
            if(dist(*temp_point_swc,*temp_point_nt)<0.01)
            {
                //need to merge
                for(V3DLONG k=0;k<tree_new.size();k++)
                {
                    MyMarker* temp_point_nt_p=tree_new.at(k);
                    if(temp_point_nt_p->parent==temp_point_nt)
                    {
                        temp_point_nt_p->parent=temp_point_swc;
                    }
                }
                tree_new[j]=tree_new.back();
                j=j-1;
                tree_new.pop_back();
                free(temp_point_nt);
            }
        }
    }
    cout<<" 1 ";
    for(V3DLONG j=0;j<tree_new.size();j++)
    {
        swc.push_back(tree_new.at(j));
    }
    NeuronTree temp_tree=vectorofMyMarker2NeuronTree(swc);
    if(1)
    {
//        cout<<" free memory ";
        //free memory
        for(V3DLONG j=0;j<tree_new.size();j++)
        {
            if(tree_new.at(j))
            {free(tree_new.at(j));}

        }
        for(V3DLONG i=0;i<tree_new.size();i++)
        {
            if(swc.at(i))
            {free(swc.at(i));}
//            free(swc.at(i));
        }
    }
    return temp_tree;
}


void merge_two_neuron(vector<MyMarker*> & swc ,NeuronTree nt)
{
    //
    vector<MyMarker*> tree_new=NeuronTree2vectorofMyMarker(nt);
    for(V3DLONG i=0;i<swc.size();i++)
    {
        MyMarker* temp_point_swc=swc.at(i);
        for(V3DLONG j=0;j<tree_new.size();j++)
        {
            MyMarker* temp_point_nt=tree_new.at(j);
            if(dist(*temp_point_swc,*temp_point_nt)<0.01)
            {
                //need to merge
                for(V3DLONG k=0;k<tree_new.size();k++)
                {
                    MyMarker* temp_point_nt_p=tree_new.at(k);
                    if(temp_point_nt_p->parent==temp_point_nt)
                    {
                        temp_point_nt_p->parent=temp_point_swc;
                    }
                }
                tree_new[j]=tree_new.back();
                j=j-1;
                tree_new.pop_back();
                free(temp_point_nt);
            }
        }
    }
    for(V3DLONG j=0;j<tree_new.size();j++)
    {
        swc.push_back(tree_new.at(j));
    }
}
NeuronTree readSWC_file1(const QString& filename)
{
    NeuronTree nt;
    nt.file = QFileInfo(filename).absoluteFilePath();
    QFile qf(filename);
    if (! qf.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return nt;
    }
    int count = 0;
    QList <NeuronSWC> listNeuron;
    QHash <int, int>  hashNeuron;
    listNeuron.clear();
    hashNeuron.clear();
    QString name = "";
    QString comment = "";
    while (! qf.atEnd())
    {
        char _buf[1000], *buf;
        qf.readLine(_buf, sizeof(_buf));
        for (buf=_buf; (*buf && *buf==' '); buf++); //skip space
        //  add #name, #comment
        if (buf[0]=='\0')	continue;
        if (buf[0]=='#')
        {
            if (buf[1]=='n'&&buf[2]=='a'&&buf[3]=='m'&&buf[4]=='e'&&buf[5]==' ')
                name = buf+6;
            if (buf[1]=='c'&&buf[2]=='o'&&buf[3]=='m'&&buf[4]=='m'&&buf[5]=='e'&&buf[6]=='n'&&buf[7]=='t'&&buf[8]==' ')
                comment = buf+9;

            continue;
        }
        count++;
        NeuronSWC S;
        QStringList qsl = QString(buf).trimmed().split(" ",QString::SkipEmptyParts);
        if (qsl.size()==0)   continue;
        for (int i=0; i<qsl.size(); i++)
        {
            qsl[i].truncate(99);
            if (i==0) S.n = qsl[i].toInt();
            else if (i==1) S.type = qsl[i].toInt();
            else if (i==2) S.x = qsl[i].toFloat();
            else if (i==3) S.y = qsl[i].toFloat();
            else if (i==4) S.z = qsl[i].toFloat();
            else if (i==5) S.r = qsl[i].toFloat();
            else if (i==6) S.pn = qsl[i].toInt();
            //the ESWC extension, by PHC, 20120217
            else if (i==7) S.seg_id = qsl[i].toInt();
            else if (i==8) S.level = qsl[i].toInt();
    //change ESWC format to adapt to flexible feature number, by WYN, 20150602
            else
        S.fea_val.append(qsl[i].toFloat());
       }
        //if (! listNeuron.contains(S)) // 081024
        {
            listNeuron.append(S);
            hashNeuron.insert(S.n, listNeuron.size()-1);
        }
    }
    if (listNeuron.size()<1)
        return nt;
    //now update other NeuronTree members
    nt.n = 1; //only one neuron if read from a file
    nt.listNeuron = listNeuron;
    nt.hashNeuron = hashNeuron;
    nt.color = XYZW(0,0,0,0); /// alpha==0 means using default neuron color, 081115
    nt.on = true;
    nt.name = name.remove('\n'); if (nt.name.isEmpty()) nt.name = QFileInfo(filename).baseName();
    nt.comment = comment.remove('\n');
    return nt;
}

NeuronTree vectorofMyMarker2NeuronTree( vector<MyMarker*> swc )
{
    //copy  vector<MyMarker*> to NeuronTree
    // change two  listNeuron
    NeuronTree nt;
//    map<MyMarker*, int> ind;
//    QHash <int, int>  hashNeuron;
//    hashNeuron.clear();
//    for(int i = 0; i < swc.size(); i++)
//        ind[swc[i]] = i;

//    for(int i = 0; i < swc.size(); i++)
//    {
//        MyMarker * marker = swc[i];
//        int parent_id;
//        if(marker->parent == 0) parent_id = -1;
//        else parent_id = ind[marker->parent];
//        NeuronSWC S;
//        S.n = i;
//        S.type =marker->type;
//        S.x=marker->x;
//        S.y=marker->y;
//        S.z=marker->z;
//        S.radius=marker->radius;
//        S.parent=parent_id;
//        nt.listNeuron.append(S);
//        hashNeuron.insert(i, nt.listNeuron.size()-1);
//    }
//    nt.hashNeuron = hashNeuron;

    QString tempname="tempname_for_transfer_2.swc";
    saveSWC_file1(tempname.toStdString(),swc);
    nt=readSWC_file1(tempname);

    return nt;
}
#define TMI_Data
#ifdef TMI_Data
LandmarkList Readdata_from_TMI_result(QString img_name)
{
    LandmarkList curlist;
    //input image name return LandmarkList
    //v3dhandle curwin = callback.currentImageWindow();
    //img_name=callback.getImageName(curwin);
    QFileInfo fileinfo(img_name);
    QString path_tp=fileinfo.path();
//    cout<<"path_tp before "<<path_tp.toUtf8().data()<<endl;
    path_tp.replace("images","enhanced");//replace dir
    path_tp.append("//");
    path_tp.append(fileinfo.baseName());
    path_tp.append("result");

    QDir dir;
    if(!dir.exists(path_tp))
    {
        QString errormsg="dir don't exist ";
        errormsg.append(path_tp);

        v3d_msg(errormsg);
        return curlist;
    }
//    cout<<"path_tp after append "<<path_tp.toUtf8().data()<<endl;
    //D:\dateset\TMI_data\BN_noisy\images\BN5_1.tif
    //‪D:\dateset\TMI_data\BN_noisy\enhanced\BN5_1result\1030_BN5_1_yuantu.marker
    //

    QString marker_filename=path_tp;
    marker_filename.append("//");
    marker_filename.append("1030_");
    marker_filename.append(fileinfo.baseName());
    marker_filename.append("_yuantu.marker");
    cout<<"marker_filename"<<marker_filename.toUtf8().data()<<endl;
//        writeMarker_file(marker_filename,LandmarkListQList_ImageMarker(tip_true_list));
    curlist=QList_ImageMarker2LandmarkList(readMarker_file(marker_filename));
    RGBA8 white;
    white.r=255;white.g=255;white.b=255;
    for(int i=0;i<curlist.size();i++)
    {
        curlist[i].color.r=0;
        curlist[i].color.g=255;
        curlist[i].color.b=0;
    }
//
    return curlist;
}
LocationSimple Nearest_Match(LocationSimple point_a,LandmarkList curlist)
{
    //input a point output the nearest tip point
    double small_dist=1000000;
    LocationSimple temp_point;

    if(curlist.size()==0)
    {
        temp_point.x=-1;
        temp_point.y=-1;
        temp_point.z=-1;
        v3d_msg("error ,can't find tip points");
        cout<<"error ,can't find tip points"<<endl;
        return temp_point;
    }
    for(V3DLONG i=0;i<curlist.size();i++)
    {
        LocationSimple cur_point=curlist.at(i);
        double temp_dist=sqrt((cur_point.x - point_a.x)*(cur_point.x - point_a.x) + (cur_point.y - point_a.y)*(cur_point.y - point_a.y) + (cur_point.z - point_a.z)*(cur_point.z - point_a.z));
        if(temp_dist<small_dist)
        {
            small_dist=temp_dist;
            temp_point=cur_point;
        }
    }
    return temp_point;

}

//LandmarkList Match_leaf_tip(LandmarkList leaf_swc,LandmarkList real_tip,double threshold=5,bool ignore_z=false)
//{
//    if(leaf_swc.isEmpty())
//    {
//        v3d_msg("Match_leaf_tipthe input leaf_swc isempty");
//        return leaf_swc;
//    }
//    if(real_tip.isEmpty())
//    {
//        v3d_msg("Match_leaf_tipthe input real_tip isempty");
//        return leaf_swc;
//    }

//    for(V3DLONG num=0;num<leaf_swc.size();num++)
//    {
//        LocationSimple leafpoint;
//        leafpoint.x=leaf_swc.at(num).x;
//        leafpoint.y=leaf_swc.at(num).y;
//        leafpoint.z=leaf_swc.at(num).z;
//        leafpoint.name="leaf_need_prun";
//        for(V3DLONG num2=0;num2<leaf_swc.size();num2++)
//        {

//        }


//    }




//}

#endif

//

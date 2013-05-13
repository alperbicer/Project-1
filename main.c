#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FREESPACE 0
struct student {
    int student_number;
    char name[30];
    int grade;
    int semester_credit;
    float gpa;
};
struct  lesson{
    int course_code;
    char course_name[25];
    int credit;
    char instructor[30];
    char classroom_name[20];
};
struct note{
    int course_code;
    int student_number;
    int student_point;
};
typedef struct student Student;
typedef struct lesson Lesson;
typedef struct note Note;

int select_menu();
int sayi_al(int,int);
void ogrenci_ekle(FILE *);
int ogrenci_bilgileri_listele(FILE *);
void ogrenci_ders_bilgileri_listele(FILE *,int,FILE *);
void siniftaki_ogrencileri_listele(FILE *);
void basari_istatiskleri_listele(FILE *);
int ders_bilgileri_listele(FILE *);
void dersi_alan_ogrencileri_listele(FILE *,int,FILE *);
void donemlik_ders_bilgileri_listele(FILE *);
void yeni_ders_ekle(FILE *);
void change_instructor_and_classroom_name(FILE *);
void save_new_student_to_lesson(FILE *,FILE *,FILE *);
void kayit_sil(FILE *,FILE *);
int main()
{
    FILE *fptr;
    FILE *fptr2;
    FILE *fptr3;
	int secenek,number;

        if((fptr=fopen("dersler.dat","rb+"))==NULL)
            printf("Dersler dosyasi acilamadi.\n");

        else if((fptr2=fopen("ogrenciler.dat","rb+"))==NULL)
            printf("Ogrenciler dosyasi acilamadi.\n");

        else if((fptr3=fopen("notlar.txt","a+"))==NULL)
            printf("Notlar dosyasi acilamadi.\n");
        else
        {
             do {
                secenek=select_menu();
                switch(secenek) {
                    case 1:
                        ogrenci_bilgileri_listele(fptr2);
                        break;
                    case 2:
                        number=ogrenci_bilgileri_listele(fptr2);
                        ogrenci_ders_bilgileri_listele(fptr3,number,fptr);
                        break;
                    case 3:
                        siniftaki_ogrencileri_listele(fptr2);
                        break;
                    case 4:
                        basari_istatiskleri_listele(fptr2);
                        break;
                    case 5:
                        ders_bilgileri_listele(fptr);
                        break;
                    case 6:
                        number=ders_bilgileri_listele(fptr);
                        dersi_alan_ogrencileri_listele(fptr3,number,fptr2);
                        break;
                    case 7:
                        donemlik_ders_bilgileri_listele(fptr);
                        break;
                    case 8:
                        ogrenci_ekle(fptr2);
                        break;
                    case 9:
                        yeni_ders_ekle(fptr);
                        break;
                    case 10:
                        change_instructor_and_classroom_name(fptr);
                        break;
                    case 11:
                        save_new_student_to_lesson(fptr,fptr2,fptr3);
                        break;
                    case 12:
                        kayit_sil(fptr2,fptr3);
                        break;
                    }
                } while(secenek!=13);
        }
    return 0;
}

int select_menu(void)
{
    int secim;

    printf("\n");
    printf("            1.  Ogrenci bilgilerini listele\n");
    printf("            2.  Bir ogrencinin bilgileri ve bu donem aldigi dersleri listele\n");
    printf("            3.  Siniflardaki ogrencileri listele\n");
    printf("            4.  Bolumlerin donemlik basari istatisklerini listele\n");
    printf("            5.  Bir dersin bilgilerini listele\n");
    printf("            6.  Bir dersin bilgilerini ve kac ogrencinin aldigini listele\n");
    printf("            7.  Donemlik verilen derslerin bilgilerini listele\n");
    printf("            8.  Yeni ogrenci ekle\n");
    printf("            9.  Yeni bir ders ekle\n");
    printf("            10. Dersin, ogretim uyesini ve dersligini degistir\n");
    printf("            11. Bir dersi alan ogrencilerin notlarini gir \n");
    printf("            12. Ogrenci kaydi sil\n");
    printf("            13. Cikis\n");
    printf("\n");
    do {
        printf("\nSeciminizi Giriniz:");
        scanf("%d", &secim);
    } while(secim<1 || secim>13);

    return secim;
}

int sayi_al(int alt_sinir,int ust_sinir)
{
    int sayi;
    do
    {
        scanf("%d",&sayi);
        if(sayi<alt_sinir || sayi>ust_sinir)
            printf("\nYanlis deger girdiniz, tekrar giriniz:");
    }while(sayi<alt_sinir || sayi>ust_sinir);
    return sayi;
}

int ogrenci_bilgileri_listele(FILE *fptr)
{
    Student one_student;
    int number;
    int register_number;
    printf("Listelemek istediginiz ogrencinin numarasini giriniz:");
    number=sayi_al(100,999);
    printf("\n");
    register_number=number-100;

    fseek(fptr,register_number*sizeof(Student),SEEK_SET);
    fread(&one_student,sizeof(Student),1,fptr);
    if(one_student.student_number==FREESPACE)   //Bu satir bos ise ekrana bos oldugunu belirten bir satir girdim
        printf("Aradiginiz ogrenciye ait bilgi bulunmamaktadir\n");
    else
        {
            printf("Ogr No   Ad Soyad    Sinif   Kredi Say    Not Ort   \n");
            printf("------   --------    -----   ---------    -------   \n");
            printf("%-9d%-14s%-10d%-10d%.2f \n\n",one_student.student_number,one_student.name,one_student.grade,one_student.semester_credit,one_student.gpa);
        }//ust satirda 9 satir once okudugum verileri ekrana struct yardimiyla yazdirdim

        return number; // return numberi, daha sonraki parametrelerde kullanmak icin satir sayisindan tasarruf amacli kullandim
}
void ogrenci_ders_bilgileri_listele(FILE *fptr,int number,FILE *fptr2)
{
    Note one_student;
    Lesson one_lesson;
    int i;
        if(number!=FREESPACE)
        {
        printf("Bu donem aldigi dersler:\n");
        printf("Ders Kodu   Ders Adi    Kredi   Notu   \n");
        printf("---------   --------    -----   ----  \n");
        while(!feof(fptr))
        {
        fscanf(fptr,"%d %d %d",&one_student.course_code,&one_student.student_number,&one_student.student_point);
        //txt formatli dosyadan fscanf yardimi ile gerekli olan bilgileri okudum
        if(one_student.student_number==number) //bir onceki parametreden donen numara,notlar.txt dosyasindan herhangi bir numaraya esit olmasi durumunda
        {//kayitli bir ogrenci bilgisi buldugumuz anlamina gelir
            for(i=0;i<80;i++) //dosyadaki ders sayisini  bildigimiz icin sirayla o dersin kodu yardimiyla bilgilerini ariyoruz.
            {
                fseek(fptr2,i*sizeof(Lesson),SEEK_SET);
                fread(&one_lesson,sizeof(Lesson),1,fptr2);
                if(one_lesson.course_code==one_student.course_code) // gerekli olan butun bilgileri bulduktan sonra ekrana yazdirabiliriz
                printf("%-12d%-14s%-7d%d \n\n",one_student.course_code,one_lesson.course_name,one_lesson.credit,one_student.student_point);
            }
        }
        }
        }
    rewind(fptr); //dosyamizin imlecini basa dondurmemizi sagliyor
}

void siniftaki_ogrencileri_listele(FILE *fptr)
{
    FILE *fptr2;
    if((fptr2=fopen("gecici.dat","wb+"))==NULL)
        printf("Dosya acilamadi");


    Student one_student;
    int i,sinif,counter=0,gpa_tutucu,credit_tutucu,j,tutucu,toplam_credit=0;
    float genel_gpa=0;
    int sinir_counter=0;
    printf("Bilgilerini gormek istediginiz sinifi giriniz:");
    sinif=sayi_al(1,4);
    for(i=0;i<900;i++)
    {
        fseek(fptr,i*sizeof(Student),SEEK_SET);
        fread(&one_student,sizeof(Student),1,fptr);

        if(one_student.student_number!=0)
        {
            if(one_student.grade==sinif)
            {
                fwrite(&one_student,sizeof(Student),1,fptr2);
                counter++;
            }
        }
    }
    for(j=0;j<counter;j++)
    {
        gpa_tutucu=0;
        credit_tutucu=0;
        for(i=0;i<counter;i++)
    {

        fseek(fptr2,sizeof(Student)*i,SEEK_SET);
        fread(&one_student,sizeof(Student),1,fptr2);
        if(one_student.student_number!=0)
        {
           if(one_student.gpa>=gpa_tutucu)
        {
            if(one_student.gpa==gpa_tutucu)
            {
                if(one_student.semester_credit>=credit_tutucu)
                {
                    tutucu=i;
                    gpa_tutucu=one_student.gpa;
                    credit_tutucu=one_student.semester_credit;
                }
            }
            else
            {
                tutucu=i;
                gpa_tutucu=one_student.gpa;
                credit_tutucu=one_student.semester_credit;
            }
        }
        }    }
                    fseek(fptr2,sizeof(Student)*tutucu,SEEK_SET);
                    fread(&one_student,sizeof(Student),1,fptr2);
                    printf("Sira No     Ogr No     Ad Soyad     Sinif      Kredi Say      Not Ort\n");
                    printf("------      ------     --------     -----      ---------      -------\n");
                    printf("   %-11d%-9d%-15s%-13d%-12d%.2f \n",j+1,one_student.student_number,one_student.name,one_student.grade,one_student.semester_credit,one_student.gpa);
                    genel_gpa+=one_student.gpa*one_student.semester_credit;
                    toplam_credit+=one_student.semester_credit;
                    if(one_student.gpa<60)
                         sinir_counter++;
                    one_student.student_number=0;
                    fseek(fptr2,sizeof(Student)*tutucu,SEEK_SET);
                    fwrite(&one_student,sizeof(Student),1,fptr2);
    }
    printf("Sinifin genel not ortalamasi:%5.2f\n",(float)genel_gpa/toplam_credit);
    printf("Donemlik agirlikli not ortalamasi 60 in altinda olan ogr sayisi ve yuzdesi:\n%-3d%%%.2f\n",sinir_counter,(float)sinir_counter/counter*100);
    fclose(fptr2);
    remove("gecici.dat");
}

void basari_istatiskleri_listele(FILE *fptr)
{
    Student one_student;
    int i,counter[10],sayac=0;
    for(i=0;i<10;i++)
        counter[i]=0;
    for(i=0;i<=899;i++)
    {
        fseek(fptr,sizeof(Student)*i,SEEK_SET);
        fread(&one_student,sizeof(Student),1,fptr);
        if(one_student.student_number!=0)
        {
        if(one_student.gpa>=90)
            counter[9]++;
        else if(one_student.gpa>=80)
            counter[8]++;
        else if(one_student.gpa>=70)
            counter[7]++;
        else if(one_student.gpa>=60)
            counter[6]++;
        else if(one_student.gpa>=50)
            counter[5]++;
        else if(one_student.gpa>=40)
            counter[4]++;
        else if(one_student.gpa>=30)
            counter[3]++;
        else if(one_student.gpa>=20)
            counter[2]++;
        else if(one_student.gpa>=10)
            counter[1]++;
        else
            counter[0]++;
        sayac++;
        }
    }
        printf("Not Araligi      Ogrenci Sayisi      Ogrenci Yuzdesi\n");
        printf("-----------      --------------      ---------------\n");
        for(i=9;i>=0;i--)
        {
            printf("%5d-%-18d%-17d%%%.2f\n",(i+1)*10,i*10,counter[i],(float)counter[i]/sayac*100);
        }
}
int ders_bilgileri_listele(FILE *fptr)
{
    Lesson one_lesson;
    int code;
    printf("Bilgilerini goruntulemek istediginiz dersin kodunu giriniz <10-89>:");
    code=sayi_al(10,89);
    printf("\n");
    fseek(fptr,sizeof(Lesson)*(code-1),SEEK_SET);
    fread(&one_lesson,sizeof(Lesson),1,fptr);
    if(one_lesson.course_code!=0)
    {
        printf("Ders Kodu       Ders Adi       Kredi      Ogretim Uyesi       Derslik\n");
        printf("---------       --------       -----      -------------       -------\n");
        printf("   %-13d%-17s%-9d%-22s%-15s\n",one_lesson.course_code,one_lesson.course_name,one_lesson.credit,one_lesson.instructor,one_lesson.classroom_name);
    }
    else
        printf("Ders kodu henuz tanimlanmamistir!\n");
        return code;
}

void dersi_alan_ogrencileri_listele(FILE *fptr,int number,FILE *fptr2)
{
    Note one_note;
    Student one_student;
    int i,counter=0,genel_gpa=0,note_counter=0;
    printf("Bu Donem Alan Ogrenciler:\n");
    while(!feof(fptr))
    {
      fscanf(fptr,"%d%d%d",&one_note.course_code,&one_note.student_number,&one_note.student_point);
      if(one_note.course_code==number)
      {
          printf("Ogr No     Ad Soyad    Sinif   Notu\n");
          printf("------     --------    -----   ----\n");
          counter++;
          for(i=0;i<900;i++)
          {
              fseek(fptr2,sizeof(Student)*i,SEEK_SET);
              fread(&one_student,sizeof(Student),1,fptr2);
              if(one_note.student_number==one_student.student_number)
              {
                  printf(" %-10d%-14s%-6d%-3d\n",one_student.student_number,one_student.name,one_student.grade,one_note.student_point);
                  if(one_note.student_point<60)
                    note_counter++;
                  genel_gpa+=one_student.gpa;
                  break;
              }
          }
      }
    }
    printf("Dersi alan ogrenci sayisi: %d\n",counter);
    printf("Dersin genel not ortalamasi: %.2f\n",(float)genel_gpa/counter);
    printf("Notu 60 in altinda olan ogr sayisi ve yuzdesi: %d %.2f\n",note_counter,(float)note_counter/counter*100);
    rewind(fptr);
}

void donemlik_ders_bilgileri_listele(FILE *fptr)
{
    Lesson one_lesson;
    int code,i,counter=0;
    printf("Bilgilerini goruntulemek istediginiz dersin donemini giriniz <1-8>:");
    code=sayi_al(1,8);
    printf("\n");
    for(i=(code*10)-1;i<(code*10)+9;i++)
    {
    fseek(fptr,sizeof(Lesson)*i,SEEK_SET);
    fread(&one_lesson,sizeof(Lesson),1,fptr);
    if(one_lesson.course_code!=0)
    {
        printf("Ders Kodu       Ders Adi       Kredi      Ogretim Uyesi       Derslik\n");
        printf("---------       --------       -----      -------------       -------\n");
        printf("   %-13d%-17s%-9d%-22s%-15s\n",one_lesson.course_code,one_lesson.course_name,one_lesson.credit,one_lesson.instructor,one_lesson.classroom_name);
        counter+=one_lesson.credit;
    }
    }
    if(counter==0)
        printf("Bu doneme ait ders bilgisi henuz girilmemistir! \n");
    printf("Derslerin kredi toplami: %d\n",counter);
}

void ogrenci_ekle(FILE *fptr)
{
    Student one_student;
    int number;
    int register_number;

    printf("Eklemek istediginiz ogrencinin numarasini giriniz:");
    number=sayi_al(100,999);

    register_number=number-100; // numarayi dosyadan okunur hale  getirdim

    fseek(fptr,register_number*sizeof(Student),SEEK_SET); // Girilen numaranin satir basina getirdim
    fread(&one_student,sizeof(Student),1,fptr); //fptr den Student boyutunda one_student struct ina veri okudum
    if(one_student.student_number==FREESPACE) {  //bu numarali ogrenci olup olmadigini kontrol ettim
        one_student.student_number=number;
        printf("Ogrencinin adini giriniz:");
        fflush(stdin);
        gets(one_student.name);
        printf("Ogrencinin sinifini giriniz:");
        scanf("%d",&one_student.grade);     //bu satira kadar verileri structa atadim
        fseek(fptr,(register_number)*sizeof(Student),SEEK_SET); //Tekrar girilen satirin basina geldim
        fwrite(&one_student,sizeof(Student),1,fptr);    //structtaki degiskenleri dosyaya yazdim
        one_student.semester_credit=0;  // kredi ve agno larini da sifirladim
        one_student.gpa=0;
        printf("Ekleme islemi basari ile gerceklestirildi.\n");
    }
    else
        printf("Girdiginiz ogrenci numarasina sahip ogrenci zaten bulunmaktadir.\n");   //ogrenci varsa tekrar ustune yazilmasini onleme
    return;
}

void yeni_ders_ekle(FILE *fptr)
{
    Lesson one_lesson;
    int i,number;
    printf("Eklemek istediginiz dersin kodunu giriniz: ");
    number=sayi_al(1,8);
    printf("\n");
    for(i=(number*10)-1;i<(number*10)+9;i++)
    {
       fseek(fptr,sizeof(Lesson)*i,SEEK_SET);
       fread(&one_lesson,sizeof(Lesson),1,fptr);
       if(one_lesson.course_code==0)
       {
           printf("Eklenecek dersin kodu: %d\n",i+1);
           one_lesson.course_code=i+1;
           printf("Eklenecek dersin adini giriniz: ");
           fflush(stdin);
           gets(one_lesson.course_name);
           printf("Eklenecek dersin kredisini giriniz: ");
           scanf("%d",&one_lesson.credit);
           printf("Eklenecek dersi veren ogretim uyesinin adini giriniz: ");
           fflush(stdin);
           gets(one_lesson.instructor);
           printf("Eklenecek dersin dersligini giriniz: ");
           fflush(stdin);
           gets(one_lesson.classroom_name);
           fseek(fptr,sizeof(Lesson)*i,SEEK_SET);
           fwrite(&one_lesson,sizeof(Lesson),1,fptr);
           printf("Guncelleme islemi basari ile tamamlandi\n");
           break;
       }
    }
}

void change_instructor_and_classroom_name(FILE *fptr)
{
    Lesson one_lesson;
    int code,i;
    printf("Bilgilerini degistirmek istediginiz dersin kodunu giriniz: ");
    code=sayi_al(10,89);
        fseek(fptr,sizeof(Lesson)*(code-1),SEEK_SET);
        fread(&one_lesson,sizeof(Lesson),1,fptr);
        if(one_lesson.course_code==0)
            printf("Girdiginiz koda ait kayitli ders bulunmamaktadir\n");
        else
        {
            printf("Dersin yeni ogretim uyesini giriniz: ");
            fflush(stdin);
            gets(one_lesson.instructor);
            printf("Dersin yeni dersligini giriniz: ");
            fflush(stdin);
            gets(one_lesson.classroom_name);
            fseek(fptr,sizeof(Lesson)*(code-1),SEEK_SET);
            fwrite(&one_lesson,sizeof(Lesson),1,fptr);
            printf("Guncelleme islemi basari ile tamamlandi\n");
        }
}
void save_new_student_to_lesson(FILE *fptr,FILE *fptr2,FILE *fptr3)
{
    if((fptr2=fopen("notlar.txt","a+"))==NULL)
    printf("Notlar.txt dosyasi acilamadi");
    else
    {
        Note one_note;
        Lesson one_lesson;
        Student one_student;
        int code,secenek,counter=0,credit;
        char go_on;
        int number;
        printf("Ogrenci eklemek istediginiz dersin kodunu giriniz: ");
        code=sayi_al(10,89);
            fseek(fptr,sizeof(Lesson)*(code-1),SEEK_SET);
            fread(&one_lesson,sizeof(Lesson),1,fptr);
            if(one_lesson.course_code==0)
                printf("Girdiginiz koda ait kayitli ders bulunmamaktadir\n");
            else
            {
                rewind(fptr);
                one_note.course_code=one_lesson.course_code;
                do
                {
                    printf("Derse eklemek istediginiz ogrencinin numarasini giriniz: ");
                    while(counter==0)
                        {
                            number=sayi_al(100,999);
                            fseek(fptr2,sizeof(Student)*(number-100),SEEK_SET);
                            fread(&one_student,sizeof(Student),1,fptr2);
                            if(one_student.student_number==number)
                            {
                                one_note.student_number=one_student.student_number;
                                counter=1;
                            }
                            else
                                printf("Girilen numarada kayitli ogrenci bulunmamaktadir! Lutfen tekrar giriniz: ");

                        }
                    printf("Ogrencinin aldigi notu giriniz: ");
                    scanf("%d",&one_note.student_point);
                    printf("Bu ders icin baska ogrenci girilecek mi ?<e/E>: ");
                    fflush(stdin);
                    scanf("%c",&go_on);
                    fprintf(fptr3,"%-3d%-4d%d\n",one_note.course_code,one_note.student_number,one_note.student_point);
                    credit=one_student.semester_credit+one_lesson.credit;
                    one_student.gpa=((one_student.gpa*one_student.semester_credit)+(one_lesson.credit*one_note.student_point))/credit;
                    one_student.semester_credit=credit;
                    fseek(fptr2,sizeof(Student)*(number-100),SEEK_SET);
                    fwrite(&one_student,sizeof(Student),1,fptr2);
                    counter=0;
                 }while(go_on=='e' || go_on=='E');
           }
    fclose(fptr3);
    }

}

void kayit_sil(FILE *fptr,FILE *fptr2)
{
    fclose(fptr2);
    FILE *fptr3;
    if((fptr2=fopen("notlar.txt","a+"))==NULL)
        printf("Notlar.txt dosyasi acilamadi");
    else if((fptr3=fopen("gecici.txt","a+"))==NULL)
        printf("Gecici.txt dosyasi acilamadi!\n");

    else
    {
        rewind(fptr2);
        Student one_student;
        Note one_note;
        int number,code;
        printf("Kaydini silmek istediginiz ogrencinin numarasini giriniz: ");
        do
        {
        number=sayi_al(100,999);
        fseek(fptr,sizeof(Student)*(number-100),SEEK_SET);
        fread(&one_student,sizeof(Student),1,fptr);
        if(one_student.student_number==0)
        {
            printf("Girdiginiz numarada ogrenci bulunmamaktadir.Tekrar giris yapiniz: ");
            number=0;
        }
        }while(number==0);
        while(!feof(fptr2))
        {
            fscanf(fptr2,"%d%d%d ",&one_note.course_code,&one_note.student_number,&one_note.student_point);
            if(one_note.student_number!=one_student.student_number )
            {
                fprintf(fptr3,"%-3d%-4d%d\n",one_note.course_code,one_note.student_number,one_note.student_point);
            }
        }
        one_student.gpa=0;
        one_student.grade=0;
        one_student.semester_credit=0;
        one_student.student_number=0;
        fseek(fptr,sizeof(Student)*(number-100),SEEK_SET);
        fwrite(&one_student,sizeof(Student),1,fptr);
        fclose(fptr3);
        fclose(fptr2);
        remove("notlar.txt");
        rename("gecici.txt","notlar.txt");
        printf("Ogrenci bilgileri basariyla silinmistir!\n");
    }
}

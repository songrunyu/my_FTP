#include <stdio.h>


main()
{
 int a,b,c,d,e;
 for (a=1; a<=5; a++)
   {
    for (b=1; b<=5; b++)
      {
       if(b==a)
         continue;
       for(c=1; c<=5; c++)
         {
          if(c==b||c==a)
            continue;
          for(d=1; d<=5; d++)
            {
             if(d==c||d==b||d==a)
               continue;
             for(e=1; e<=5; e++)
               {
                if(e==d||e==c||e==b||e==a)
                  continue;
                if(((b==2)+(a==3))==1 && ((b==2)+(e==4))==1 && ((c==1)+(d==2))==1 && ((c==5)+(d==3))==1 && ((e==4)+(a==1))==1)
                  {
                   printf("a=%d b=%d c=%d d=%d e=%d\n",a,b,c,d,e);
                  }
               }
            }
         }
      }
    }
}

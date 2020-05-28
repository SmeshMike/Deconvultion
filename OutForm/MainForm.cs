using OutForm.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using SomeShit;
using static SomeShit.TrippleGen;

namespace OutForm
{
    public partial class MainForm : Form
    {

        TrippleGen forform = new TrippleGen();

        public MainForm()
        {
            InitializeComponent();

            //int ss = Convert.ToInt32(TextL.Text);


            //for (int i = 0; i < ss+1; i++)
            //{
            //    this.SignGraph.Series.Add(new System.Windows.Forms.DataVisualization.Charting.Series()
            //    {
            //        Name = "Series" + i,
            //        ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line

            //    });
            //    this.Load += Form1_Load;
            //}


            this.SignGraph.Series.Add(new System.Windows.Forms.DataVisualization.Charting.Series()
            {
                Name = "Series1" ,
                ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line

            });
            this.SignGraph.Series.Add(new System.Windows.Forms.DataVisualization.Charting.Series()
            {
                Name = "Series2",
                ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line

            }); this.SignGraph.Series.Add(new System.Windows.Forms.DataVisualization.Charting.Series()
            {
                Name = "Series3",
                ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line

            });

            this.Load += Form1_Load;
        }



        private void Form1_Load(object sender, EventArgs e)
        {
            sinPanel1.tbB.Text = Convert.ToString(0.08);
            sinPanel1.tbC.Text = Convert.ToString(Math.Round(Convert.ToDouble(sinPanel1.tbB.Text)/(Math.PI*2), 4));
        }

        List<dot> sign = new List<dot>();

        private void Start_Click_1(object sender, EventArgs e)
        {
            

            ////////////////// ОЧИСТКА ГРАФИКОВ И ФОРМЫ /////////////////////

            //SignGraph.Series[0].Points.Clear();
            //SignGraph.Series[1].Points.Clear();
            //SignGraph.Series[2].Points.Clear();
            //forform.Clear();

            ////////////////// ОПРЕДЕЛЕНИЕ ПЕРЕМЕННЫХ /////////////////////

            UInt32 _end = Convert.ToUInt32(this.end.Text);
            int length = 0;

            SinPanel tb = sinPanel1;

            //SignGraph.Series[0].Points.Clear();


            int ampl = Convert.ToInt32(sinPanel1.tbA.Text); //амлитуда
            double freq = Convert.ToDouble(sinPanel1.tbB.Text);  //частота  
            double phase = 0.0;   //фаза начальная
            UInt16 first = 0;     //начало
            UInt32 last = _end;   //конец


            if (SqWindow.Checked)
                forform.SqSinGen(ampl, freq, phase, first, last);
            else if (TrWindow.Checked)
                forform.TrSinGen(ampl, freq, phase, first, Convert.ToInt32(last));
            else if (CosWindow.Checked)
                forform.CosSinGen(ampl, freq, phase, first, last);

            forform.SignalwithWhiteNoise(Convert.ToInt32(NoisePerc.Text));
            NoiseText.Text = forform.GetCoef().ToString();

            sign = forform.SignReturn();

            ////////////////// СОЗДАНИЕ ГРАФИКОВ /////////////////////

            SignGraph.Series[0].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            SignGraph.Series[0].Color = Color.Red;
            SignGraph.Series[0].BorderWidth = 1;
            SignGraph.Series[1].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            SignGraph.Series[1].Color = Color.Black;
            SignGraph.Series[1].BorderWidth = 1;
            SignGraph.Series[2].ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            SignGraph.Series[2].Color = Color.Green;
            SignGraph.Series[2].BorderWidth = 1;




            length = forform.GetS();



            ////////////////// ПОСТРОЕНИЕ /////////////////////


            for (int i = 0; i < length; i++)
            {
                //double x;
                int x;
                x = i;
                double y = forform.GetYPoints(i);

                SignGraph.Series[0].Points.AddXY(x, y);
                                                              
               // this.SignGraph.ChartAreas[0].AxisX.Interval = length / 10;
                this.SignGraph.ChartAreas[0].AxisX.Minimum = 0;
            }

        }

        private void Corr_Click(object sender, EventArgs e)
        {
            //this.SignGraph.Series.Clear();

            int l_count = Convert.ToInt16(TextL.Text);
            int full_length = forform.GetS();

            this.Load += Form1_Load;

            //for (int i = 1; i < l_count+1; i++)
            //{
            //    this.SignGraph.Series.Add(new System.Windows.Forms.DataVisualization.Charting.Series()
            //    {
            //        Name = "Series" + i,
            //        ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line

            //    });
            //    this.Load += Form1_Load;
            //}

            //for (int i = 0; i < count; i++)
            //{
            //    SignGraph.Series[i].Points.Clear();
            //}


            corgraphtrue = forform.Corr(l_count, full_length);

            corgraphclone.AddRange(corgraphtrue);

            int iter = corgraphclone.Count();

            while (iter != full_length)
            {
                corgraphclone.Add(new dot(0, 0, Convert.ToUInt32(iter)));
                iter++;
            }

            for (int j = 0; j < full_length; j++)
            {
                int x;
                double y;
                x = j;
                y = corgraphclone[j].real_amplitude;

                SignGraph.Series[1].Points.AddXY(x, y);
            }
        }



        List<int> param = new List<int>();

        double[] fo;

        List<dot> corgraphtrue = new List<dot>();
        List<dot> corgraphclone = new List<dot>();
        private void Fourea_Click(object sender, EventArgs e)
        {

            int full_length = forform.GetS();

            int count = Convert.ToInt16(TextL.Text);

            //for (int i = 0; i < count + 1; i++)
            //{
            //    SignGraph.Series[i].Points.Clear();
            //}

            for (int i = 0; i < 2; i++)
            {
                SignGraph.Series[i].Points.Clear();
            }


            fo = new double[count];
            int[] itr = new int[count];

            for (int i = 0; i < count; i++)
            {
                var tmp = new List<dot>();
                tmp.AddRange(corgraphclone);

                for (int j = count - 1; j >= count - i; j--)            //заменяем начиная с конца элементы с нуля
                {
                    tmp[j] = new dot(0, 0, Convert.ToUInt32(count - i - 1));
                }

                forform.Fourea(tmp, full_length, -1);           //хуячим Фурьём

                double max = 0.0;

                for (int j = 0; j < full_length / 2; j++)
                {
                    if (Math.Sqrt(tmp[j].im_amplitude * tmp[j].im_amplitude + tmp[j].real_amplitude * tmp[j].real_amplitude) > max)
                    { max = Math.Sqrt(tmp[j].im_amplitude * tmp[j].im_amplitude + tmp[j].real_amplitude * tmp[j].real_amplitude); itr[i] = j; }
                }

                for (int j = 0; j < full_length; j++)
                {
                    int x;
                    double y;
                    x = j;
                    y = Math.Sqrt(tmp[j].im_amplitude * tmp[j].im_amplitude + tmp[j].real_amplitude * tmp[j].real_amplitude);

                    //SignGraph.Series[i].Points.AddXY(x, y);
                }


                fo[i] = (itr[i]) * 2 / Convert.ToDouble(full_length);
                                              
            }


            Array.Reverse(fo);
            Array.Reverse(itr);


            for (int i = 0; i < count; i++)
            {
                Console.WriteLine(i + " itr  " + itr[i] + "    fo   " + fo[i]);
            }
        }







        private void Paint_Click(object sender, EventArgs e)
        {

            this.Load += Form1_Load;


            int passcount = Convert.ToInt32(Count.Text);

            double[] newfo = new double[passcount];
            
            Console.Clear();

            int count = Convert.ToInt32(TextL.Text);
                        
            Array.Copy(fo,fo.Length - passcount,newfo,0, passcount);

            for (int i = 0; i < passcount; i++)
            {
                Console.WriteLine(i + "   " + newfo[i]);
            }


            for (int i = 0; i < 3; i++)
            {
                SignGraph.Series[i].Points.Clear();
            }


            double[] disp = new double[count];
            double f = Convert.ToDouble(sinPanel1.tbB.Text) / (2 * Math.PI);

            for (int i = 1; i <= passcount; i++)
            {
                double diff = 0.0;
                for (int j = 0; j < i; j++)
                {
                    diff += (f - newfo[j]) * (f - newfo[j]);
                }

                Console.WriteLine(i + "   f   " + f + "    fo   " + newfo[i-1] + "     " + "    disp   " + diff / i);

                disp[i - 1] = diff / i;

            }


            for (int i = 0; i < passcount; i++)
            {
                int x;
                double y;
                x = i;
                y = disp[i];

                SignGraph.Series[2].Points.AddXY(x, y);

                //this.SignGraph.ChartAreas[0].AxisX.Interval = 1;
                //this.SignGraph.ChartAreas[0].AxisX.Minimum = 0;
            }

            disp = null;
        }


        private void Clear_Click(object sender, EventArgs e)
        {
            int count = Convert.ToInt32(TextL.Text);

            //try
            //{
            //    for (int i = 0; i < count + 1; i++)
            //    {
            //        SignGraph.Series[i].Points.Clear();
            //    }
            //}
            //catch
            //{
            //    SignGraph.Series[0].Points.Clear();
            //}

          //  SignGraph.Series.Clear();
            sign.Clear();
            forform.Clear();
            param.Clear();
            fo = null;
            corgraphclone.Clear();
            Console.Clear();
            corgraphtrue.Clear();
            //this.SignGraph.Series.Add(new System.Windows.Forms.DataVisualization.Charting.Series()
            //{
            //    Name = "Series" + 2,
            //    ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line

            //});

            for (int i = 0; i < 3; i++)
            {
                SignGraph.Series[i].Points.Clear();
            }

            this.Load += Form1_Load;

        }




        private void GraphNum_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {

                int full_length = forform.GetS();
                int count = Convert.ToInt32(TextL.Text);
                this.SignGraph.Series.Clear();

                //this.SignGraph.Series.Add(new System.Windows.Forms.DataVisualization.Charting.Series()
                //{
                //    Name = "Series" ,
                //    ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line

                //});

                var tmp = new List<dot>();
                tmp.AddRange(corgraphclone);

                for (int j = count - 1; j >= Convert.ToInt32(GraphNum.Text); j--)            //заменяем начиная с конца элементы с нуля
                {
                    tmp[j] = new dot(0, 0, Convert.ToUInt32(j));
                }

                forform.Fourea(tmp, full_length, -1);

                for (int j = 0; j < full_length; j++)
                {
                    int x;
                    double y;
                    x = j;
                    y = Math.Sqrt(tmp[j].im_amplitude * tmp[j].im_amplitude + tmp[j].real_amplitude * tmp[j].real_amplitude);

                    Console.WriteLine(j + "     " + y);

                    SignGraph.Series[0].Points.AddXY(x, y);
                }

            }
        }

        private void TextL_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                this.SignGraph.Series.Clear();

                int ss = Convert.ToInt32(TextL.Text);


                for (int i = 0; i < ss + 1; i++)
                {
                    //this.SignGraph.Series.Add(new System.Windows.Forms.DataVisualization.Charting.Series()
                    //{
                    //    Name = "Series" + i,
                    //    ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line

                    //});
                    //this.Load += Form1_Load;
                }
            }
        }
    }
}
//private void Fourea_Click(object sender, EventArgs e)
//{

//    int full_length = forform.GetS();

//    int count = Convert.ToInt16(TextL.Text);

//    for (int i = 0; i < count + 1; i++)
//    {
//        SignGraph.Series[i].Points.Clear();
//    }


//    fo = new double[count];

//    for (int i = 0; i < count; i++)
//    {
//        var tmp = new List<dot>();
//        tmp.AddRange(corgraphclone);

//        for (int j = count - 1; j >= count - i; j--)            //заменяем начиная с конца элементы с нуля
//        {
//            tmp[j] = new dot(0, 0, Convert.ToUInt32(count - i - 1));
//        }

//        forform.Fourea(tmp, full_length, -1);           //хуячим Фурьём

//        double max = 0.0, itr = 0.0;

//        for (int j = 0; j < full_length / 2; j++)
//        {
//            if (Math.Sqrt(tmp[j].im_amplitude * tmp[j].im_amplitude + tmp[j].real_amplitude * tmp[j].real_amplitude) > max)
//            { max = Math.Sqrt(tmp[j].im_amplitude * tmp[j].im_amplitude + tmp[j].real_amplitude * tmp[j].real_amplitude); itr = j; }
//        }

//        //if (itr == full_length)
//        //{
//        //    itr = 0;
//        //}


//        for (int j = 0; j < full_length; j++)
//        {
//            int x;
//            double y;
//            x = j;
//            y = Math.Sqrt(tmp[j].im_amplitude * tmp[j].im_amplitude + tmp[j].real_amplitude * tmp[j].real_amplitude);

//            SignGraph.Series[i].Points.AddXY(x, y);
//        }


//        fo[i] = (itr) * 2 / Convert.ToDouble(full_length);



//        Console.WriteLine(i + " itr  " + itr + "    fo   " + fo[i]);
//    }

//    fo.Reverse();
//}

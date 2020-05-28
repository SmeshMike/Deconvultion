using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SomeShit
{
    class TrippleGen
    {

        public struct dot
        {
            public double real_amplitude; public double im_amplitude; public UInt32 x_pos;
            public dot(double param1, double param2, UInt32 param3)
            { real_amplitude = param1; im_amplitude = param2; x_pos = param3; }

        }

        List<dot> signal = new List<dot>();
        List<double> mas_cur_phase = new List<double>();
        List<double> new_signal = new List<double>();



        double current_phase;
        private double noise_energy;
        private double signal_energy;

        public TrippleGen()
        {
            noise_energy = 0.0;
            signal_energy = 0.0;
        }

        public void SqSinGen(double ampl, double samp_freq, double phase, UInt32 first, UInt32 last)
        {
            dot temp;
            


            for (UInt32 i = 0; i < last - first; i++)
            {

                double a;
                current_phase = phase + samp_freq * i;
                mas_cur_phase.Add(current_phase);

                a = ampl * Math.Sin(current_phase);
                signal_energy += a*a;

                temp = new dot(a, 0, i);

                signal.Add(temp);
            }
        }

        public void TrSinGen(double ampl, double samp_freq, double phase, int first, int last)
        {
            dot temp;

            double coef;

            for (int i = 0; i < last - first; i++)
            {
                double a;
                
                current_phase = phase + samp_freq * i;
                mas_cur_phase.Add(current_phase);
                coef = 1 - 2 * Math.Abs(Convert.ToDouble(i - last / 2) / Convert.ToDouble(last));


                a = ampl * Math.Sin(current_phase) * coef;
                signal_energy += a * a;

                temp = new dot(a, 0, Convert.ToUInt32(i));

                signal.Add(temp);
            }
        }

        public void CosSinGen(double ampl, double samp_freq, double phase, UInt32 first, UInt32 last)
        {
            dot temp;



            for (UInt32 i = 0; i < last - first; i++)
            {

                double a;
                current_phase = phase + samp_freq * i;
                mas_cur_phase.Add(current_phase);
                double coef = 0.54 + 0.46 * Math.Cos(2 * Math.PI * (i - last / 2) / last);

                a = ampl * Math.Sin(current_phase)* coef;
                signal_energy += a * a;

                temp = new dot(a, 0, i);

                signal.Add(temp);
            }
        }

        public List<dot> SignReturn()
        {
            return signal;        
        }

        public void Fourea(List<dot> sig, int n, int s)
        {
            int i, j, istep;
            int m, mmax;
            double r, r1, theta, w_r, w_i, temp_r, temp_i;
            double pi = 3.1415926f;

            r = pi * s;
            j = 0;
            for (i = 0; i < n; i++)
            {
                if (i < j)
                {

                    temp_r = sig[j].real_amplitude;
                    temp_i = sig[j].im_amplitude;

                    sig[j] = new dot(sig[i].real_amplitude, sig[i].im_amplitude, sig[j].x_pos);
                    sig[i] = new dot(temp_r, temp_i, sig[i].x_pos);
                }
                m = n >> 1;
                while (j >= m) { j -= m; m = (m + 1) / 2; }
                j += m;
            }
            mmax = 1;
            while (mmax < n)
            {
                istep = mmax << 1;
                r1 = r / (double)mmax;
                for (m = 0; m < mmax; m++)
                {
                    theta = r1 * m;
                    w_r = (double)Math.Cos((double)theta);
                    w_i = (double)Math.Sin((double)theta);
                    for (i = m; i < n; i += istep)
                    {

                        j = i + mmax;
                        temp_r = w_r * sig[j].real_amplitude - w_i * sig[j].im_amplitude;
                        temp_i = w_r * sig[j].im_amplitude + w_i * sig[j].real_amplitude;

                        sig[j] = new dot(sig[i].real_amplitude - temp_r, sig[i].im_amplitude - temp_i, sig[j].x_pos);

                        sig[i] = new dot(sig[i].real_amplitude + temp_r, sig[i].im_amplitude + temp_i, sig[i].x_pos);
                    }
                }
                mmax = istep;
            }
            if (s > 0)
                for (i = 0; i < n; i++)
                {
                    sig[i] = new dot(sig[i].real_amplitude / (Convert.ToDouble(n)), sig[i].im_amplitude / (Convert.ToDouble(n)), sig[i].x_pos);
                }

        }


        //public void SignalGen(int sin_count, double[] _ampl, double[] _phase, double[] _samp_freq, UInt32[] _first, UInt32[] _last)
        //{
        //    for (int i = 0; i < sin_count; i++)
        //    {
        //        SinGen(_ampl[i], _phase[i], _samp_freq[i], _first[i], _last[i]);
        //    }
        //}

        public void SignalwithWhiteNoise(int percent)
        {
            double[] noise = new double[signal.Count];
            Random rnd = new Random();
            double rand_part = 0;
            for (int i = 0; i < signal.Count; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    rand_part += rnd.NextDouble() * 2 - 1;
                }
                rand_part = rand_part / 10;

                noise_energy += rand_part * rand_part;
                noise[i] = rand_part;

                rand_part = 0;
            }

            for (int i = 0; i < signal.Count; i++)
            {
                signal[i] = new dot(signal[i].real_amplitude + percent * noise[i], 0, signal[i].x_pos);
            }
        }

        public List<dot> Corr(int count, int lenght)
        {

            List<dot> temp = signal;
            List<dot> CRList = new List<dot>();
            double tmp;
            dot tmp1;

            for (int m = 0; m < count; m++)
            {
                tmp = 0.0;

                for (int n = 0;  n < lenght - m - 1; n++)
                {
                    tmp += temp[n + m].real_amplitude * temp[n].real_amplitude;
                }

                tmp = tmp/(lenght - m);

                tmp1 = new dot(tmp, 0, Convert.ToUInt16(m));

                CRList.Add(tmp1);
            }

            return CRList;

        }

        public double GetYPoints(int key)
        {
            return signal[key].real_amplitude;
        }

        public int GetS()
        {
            return signal.Count;
        }
        public void Clear()
        {
            signal.Clear();
            mas_cur_phase.Clear();
            signal_energy = 0;
            noise_energy = 0;
        }
        public double GetCoef()
        {
            return Convert.ToDouble(Math.Sqrt(signal_energy / noise_energy));
        }

        public double GetEnergy()
        {
            return signal_energy;
        }

        public double Phase()
        {
            return current_phase;
        }

        public double Phase(int key)
        {
            return mas_cur_phase[key];
        }

    }
}



namespace web
{
    using System;
    using System.Collections;
    using System.IO;
    using System.Web;
    using System.Web.UI;

    public class AllowIP : UserControl
    {
        public static ArrayList listIP = new ArrayList();
        public static ArrayList listIPx = new ArrayList();

        public AllowIP()
        {
            if (listIP.Count <= 0)
            {
                StreamReader reader = new StreamReader(HttpContext.Current.Server.MapPath("~") + @"\config\country-ipv4.lst");
                while (reader.Peek() >= 0)
                {
                    string str2 = reader.ReadLine();
                    if (str2.IndexOf("allowip") != -1)
                    {
                        string[] strArray = str2.Split(new char[] { ' ' });
                        listIP.Add(strArray[0] + "|" + strArray[2]);
                    }
                }
                reader.Close();
            }
            if (listIPx.Count <= 0)
            {
                StreamReader reader2 = new StreamReader(HttpContext.Current.Server.MapPath("~") + @"\config\country-ipv4-twsg.lst");
                while (reader2.Peek() >= 0)
                {
                    string str4 = reader2.ReadLine();
                    if (str4.IndexOf("allowip") != -1)
                    {
                        string[] strArray2 = str4.Split(new char[] { ' ' });
                        listIPx.Add(strArray2[0] + "|" + strArray2[2]);
                    }
                }
                reader2.Close();
            }
        }

        private long IpToInt(string ip)
        {
            char[] separator = new char[] { '.' };
            if (ip.Split(separator).Length == 3)
            {
                ip = ip + ".0";
            }
            string[] strArray = ip.Split(separator);
            long num2 = ((long.Parse(strArray[0]) * 0x100L) * 0x100L) * 0x100L;
            long num3 = (long.Parse(strArray[1]) * 0x100L) * 0x100L;
            long num4 = long.Parse(strArray[2]) * 0x100L;
            long num5 = long.Parse(strArray[3]);
            return (((num2 + num3) + num4) + num5);
        }

        public bool ScopeIncludingIpAddress(string ipAddress)
        {
            long num = this.IpToInt(ipAddress);
            for (int i = 0; i < listIP.Count; i++)
            {
                string[] strArray = listIP[i].ToString().Split(new char[] { '|' });
                if ((this.IpToInt(strArray[0].ToString()) <= num) && (num <= this.IpToInt(strArray[1].ToString())))
                {
                    return true;
                }
            }
            return false;
        }

        public bool ScopeIncludingIpAddressx(string ipAddress)
        {
            long num = this.IpToInt(ipAddress);
            for (int i = 0; i < listIP.Count; i++)
            {
                string[] strArray = listIPx[i].ToString().Split(new char[] { '|' });
                if ((this.IpToInt(strArray[0].ToString()) <= num) && (num <= this.IpToInt(strArray[1].ToString())))
                {
                    return true;
                }
            }
            return false;
        }
    }
}

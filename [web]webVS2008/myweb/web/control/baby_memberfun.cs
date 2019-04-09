namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Xml;
    using web;

    public class baby_memberfun : UserControl
    {
        protected string strfunmenu;

        private void InitializeComponent()
        {
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new system().loadConfig(0);
            XmlDocument document = new XmlDocument();
            document.Load(base.Server.MapPath("config/menu.config"));
            XmlNodeList list = document.SelectNodes("//list");
            int num = 1;
            foreach (XmlNode node in list)
            {
                string strfunmenu;
                if (!(node.SelectSingleNode("@disable").ToString() != "true"))
                {
                    continue;
                }
                if ((num == list.Count) && ((num % 2) != 0))
                {
                    strfunmenu = this.strfunmenu;
                    this.strfunmenu = strfunmenu + "<tr><td width=80><img src=images/icon_leftmenu_blue.gif border=0>&nbsp;<a href=" + node.SelectSingleNode("@url").InnerText.ToString() + ">" + node.SelectSingleNode("@text").InnerText.ToString() + "</a></td></tr>";
                }
                else if ((num % 2) == 0)
                {
                    strfunmenu = this.strfunmenu;
                    this.strfunmenu = strfunmenu + "<td><img src=images/icon_leftmenu_blue.gif border=0>&nbsp;<a href=" + node.SelectSingleNode("@url").InnerText.ToString() + ">" + node.SelectSingleNode("@text").InnerText.ToString() + "</a></td>";
                }
                else
                {
                    strfunmenu = this.strfunmenu;
                    this.strfunmenu = strfunmenu + "<tr><td width=80><img src=images/icon_leftmenu_blue.gif border=0>&nbsp;<a href=" + node.SelectSingleNode("@url").InnerText.ToString() + ">" + node.SelectSingleNode("@text").InnerText.ToString() + "</a></td>";
                }
                num++;
            }
        }
    }
}

namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Xml;

    public class cpmenu : UserControl
    {
        protected string strmenu;

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
            XmlDocument document = new XmlDocument();
            if (base.Session["admin_id"].ToString() == "wginui")
            {
                document.Load(base.Server.MapPath("../config/cpmenu.config"));
            }
            else
            {
                document.Load(base.Server.MapPath("../config/cpmenu_kf.config"));
            }
            XmlNodeList list = document.SelectNodes("//menu");
            XmlNodeList list2 = document.SelectNodes("//list");
            int num = 0;
            foreach (XmlNode node in list)
            {
                num++;
                this.strmenu = this.strmenu + "<table width=\"146\" border=\"0\" cellspacing=\"0\" align=\"center\" cellpadding=\"0\" class=\"leftmenulist\" style=\"MARGIN-BOTTOM: 5px\">";
                this.strmenu = this.strmenu + "<TBODY><tr class=\"leftmenutext\">";
                object strmenu = this.strmenu;
                this.strmenu = string.Concat(new object[] { strmenu, "<td><a href=\"#\" onclick=\"collapse_change(", num, ")\"><img id=\"menuimg_", num, "\" src=\"../images/admin/menu_reduce.gif\" border=\"0\"></a>&nbsp;" });
                strmenu = this.strmenu;
                this.strmenu = string.Concat(new object[] { strmenu, "<a href=\"#\" onclick=\"collapse_change(", num, ")\">", node.SelectSingleNode("@text").Value, "</a></td></tr>" });
                strmenu = this.strmenu;
                this.strmenu = string.Concat(new object[] { strmenu, "<tbody id=\"menu_", num, "\"><tr class=\"leftmenutd\"><td>" });
                this.strmenu = this.strmenu + "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" class=\"leftmenuinfo\">";
                foreach (XmlNode node2 in list2)
                {
                    if ((node2.SelectSingleNode("@type").Value == node.SelectSingleNode("@type").Value) && (node2.SelectSingleNode("@disable").Value == "false"))
                    {
                        string str = this.strmenu;
                        this.strmenu = str + "<tr><td><a href=" + node2.SelectSingleNode("@url").Value + ">" + node2.SelectSingleNode("@text").Value + "</a></td></tr>";
                    }
                }
                this.strmenu = this.strmenu + "</table></td></tr></tbody></table>";
            }
        }
    }
}

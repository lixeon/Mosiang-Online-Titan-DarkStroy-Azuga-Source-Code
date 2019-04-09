namespace web.control
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using web;

    public class menunewstypelist : UserControl
    {
        protected string strtypelist;

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
            if (!this.Page.IsPostBack)
            {
                DataProviders providers = new DataProviders();
                SqlDataReader reader = providers.ExecuteSqlDataReader("select * from web_newstype where used=1");
                this.strtypelist = this.strtypelist + "<tr><td><img src=\"images/icon_leftmenu_red.gif\">&nbsp;<a href=\"newslist.aspx\">☆全部新聞☆</a></td></tr>";
                this.strtypelist = this.strtypelist + "<tr><td><img src=\"images/press_line.gif\"></td></tr>";
                while (reader.Read())
                {
                    object strtypelist = this.strtypelist;
                    this.strtypelist = string.Concat(new object[] { strtypelist, "<tr><td><img src=\"images/icon_leftmenu_red.gif\">&nbsp;<a href=\"newslist.aspx?type=", reader["id"], "\">☆", reader["name"].ToString(), "☆</a></td></tr>" });
                    this.strtypelist = this.strtypelist + "<tr><td><img src=\"images/press_line.gif\"></td></tr>";
                }
                reader.Close();
                providers.CloseConn();
            }
        }
    }
}

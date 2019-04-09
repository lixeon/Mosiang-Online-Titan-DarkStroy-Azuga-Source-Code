namespace web.control
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using web;

    public class menudownloadlist : UserControl
    {
        protected string strdownloadlist;

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
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select top 5 substring(name,1,30) as name ,link,convert(char(10),date,111) as date from web_download order by date desc");
            while (reader.Read())
            {
                this.strdownloadlist = this.strdownloadlist + "<tr><td width=\"380\" height=\"23\">";
                object strdownloadlist = this.strdownloadlist;
                this.strdownloadlist = string.Concat(new object[] { strdownloadlist, "<a href=\"", reader["link"], "\" target=\"_blank\"><img src=images/icon_leftmenu_purple.gif>&nbsp;", reader["name"], "...</a></td><td width=\"66\">", reader["date"], "</td></tr>" });
                this.strdownloadlist = this.strdownloadlist + "<tr><td colspan=\"2\"><img src=\"images/press_line.gif\" width=\"100%\" height=\"1\"></td></tr>";
            }
            reader.Close();
            providers.CloseConn();
        }
    }
}

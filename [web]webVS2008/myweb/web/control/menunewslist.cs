namespace web.control
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using web;

    public class menunewslist : UserControl
    {
        protected string strnewslist;

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
            SqlDataReader reader = providers.ExecuteSqlDataReader("select top 10 a.id as id,substring(title,1,30) as title ,type,name, convert(char(10),adddate,111) as adddate from web_news a,web_newstype b where a.type=b.id and b.used=1 order by adddate desc");
            while (reader.Read())
            {
                object strnewslist = this.strnewslist;
                this.strnewslist = string.Concat(new object[] { strnewslist, "<tr><td width=\"100%\" height=\"23\"><a href='newslist.aspx?type=", reader["type"], "' class='style32'><strong>[", reader["name"], "]</strong></a>" });
                strnewslist = this.strnewslist;
                this.strnewslist = string.Concat(new object[] { strnewslist, "<a href=\"viewnews.aspx?id=", reader["id"], "\" target=\"_blank\">", reader["title"], "</a></td><td width=\"66\">", reader["adddate"], "</td></tr>" });
                this.strnewslist = this.strnewslist + "<tr><td colspan=\"2\"><img src=\"images/press_line.gif\" width=\"100%\" height=\"1\"></td></tr>";
            }
            reader.Close();
            providers.CloseConn();
        }
    }
}

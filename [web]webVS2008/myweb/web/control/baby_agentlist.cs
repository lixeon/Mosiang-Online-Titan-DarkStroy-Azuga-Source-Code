namespace web.control
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using web;

    public class baby_agentlist : UserControl
    {
        protected string stragentlist;

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
            SqlDataReader reader = providers.ExecuteSqlDataReader("SELECT [id],[name] FROM [MHCMEMBER].[dbo].[Web_Agent] where  [state]=1");
            while (reader.Read())
            {
                this.stragentlist = this.stragentlist + "<tr><td width=\"380\" height=\"23\">";
                object stragentlist = this.stragentlist;
                this.stragentlist = string.Concat(new object[] { stragentlist, "<img src=images/icon_leftmenu_purple.gif><a href=agent.aspx?id=", reader["id"], " target=\"_blank\">&nbsp;", reader["name"], "</a></td><td width=\"66\"></td></tr>" });
            }
            reader.Close();
            providers.CloseConn();
        }
    }
}

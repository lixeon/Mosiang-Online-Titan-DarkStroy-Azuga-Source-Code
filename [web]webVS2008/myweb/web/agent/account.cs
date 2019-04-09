namespace web.agent
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class account : Page
    {
        protected Button btnlogout;
        protected string gold;

        private void btnlogout_Click(object sender, EventArgs e)
        {
            this.Session.Abandon();
            base.Response.Redirect("default.aspx");
        }

        private void InitializeComponent()
        {
            this.btnlogout.Click += new EventHandler(this.btnlogout_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isagent();
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select * from mhcmember..web_agent where userid='" + this.Session["agent_id"].ToString() + "'");
            if (reader.Read())
            {
                this.gold = reader["gold"].ToString();
            }
            reader.Close();
            providers.CloseConn();
        }
    }
}

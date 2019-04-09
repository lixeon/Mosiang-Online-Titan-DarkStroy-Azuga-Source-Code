namespace web.control
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class agent : UserControl
    {
        protected string bankinfo;
        protected string gold;
        protected string hp;
        protected string msn;
        protected string name;
        protected string nation;
        protected string other;
        protected string qq;
        protected TextBox TextBox1;
        protected TextBox TextBox2;
        protected string tp;

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
            if (base.Request.QueryString["id"] != null)
            {
                DataProviders providers = new DataProviders();
                SqlDataReader reader = providers.ExecuteSqlDataReader("select * from web_agent" + (" where id=" + int.Parse(base.Request.QueryString["id"]).ToString()));
                if (reader.Read())
                {
                    this.nation = reader["nation"].ToString();
                    this.name = reader["name"].ToString();
                    this.msn = reader["msn"].ToString();
                    this.qq = reader["qq"].ToString();
                    this.hp = reader["handphone"].ToString();
                    this.tp = reader["telephone"].ToString();
                    this.bankinfo = reader["bankinfo"].ToString();
                    this.TextBox1.Text = this.bankinfo;
                    this.other = reader["other"].ToString();
                    this.TextBox2.Text = this.other;
                    this.gold = reader["gold"].ToString();
                }
                reader.Close();
                providers.CloseConn();
            }
        }
    }
}

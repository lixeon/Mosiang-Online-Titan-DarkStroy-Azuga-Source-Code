namespace web
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;

    public class baby_psinfo : Page
    {
        protected string stradddate;
        protected string strcontent;
        protected string strenddate;
        protected string strtitle;
        protected string strwinprice;
        protected string strwinuserid;

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
            if (this.Session["userid"] == null)
            {
                base.Response.Redirect("login.aspx?returnurl=" + base.Request.ServerVariables["SCRIPT_NAME"]);
            }
            if (base.Request.QueryString["id"] != null)
            {
                DataProviders providers = new DataProviders();
                SqlDataReader reader = providers.ExecuteSqlDataReader("select * from web_publicsale" + (" where id=" + int.Parse(base.Request.QueryString["id"]).ToString()));
                if (reader.Read())
                {
                    this.strtitle = reader["title"].ToString();
                    this.stradddate = reader["adddate"].ToString();
                    this.strenddate = reader["enddate"].ToString();
                    this.strwinuserid = reader["winuserid"].ToString();
                    string str2 = this.strwinuserid.Substring(0, 1).ToString();
                    string str3 = this.strwinuserid.Substring(3).ToString();
                    this.strwinuserid = str2 + "**" + str3;
                    this.strwinprice = reader["winprice"].ToString();
                    this.strcontent = reader["content"].ToString();
                    reader.Close();
                    providers.CloseConn();
                }
                else
                {
                    reader.Close();
                    providers.CloseConn();
                    base.Response.Redirect("publicsale.aspx");
                }
            }
            else
            {
                base.Response.Redirect("publicsale.aspx");
            }
        }
    }
}

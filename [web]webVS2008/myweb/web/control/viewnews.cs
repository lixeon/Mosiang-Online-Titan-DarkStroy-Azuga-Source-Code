namespace web.control
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using web;

    public class viewnews : UserControl
    {
        protected string stradddate;
        protected string strauthor;
        protected string strclick;
        protected string strcontent;
        protected string strtitle;

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
                SqlDataReader reader = providers.ExecuteSqlDataReader("select * from web_news" + (" where id=" + int.Parse(base.Request.QueryString["id"]).ToString()));
                if (reader.Read())
                {
                    this.strtitle = reader["title"].ToString();
                    this.stradddate = reader["adddate"].ToString();
                    this.strauthor = reader["author"].ToString();
                    this.strclick = reader["clicks"].ToString();
                    this.strcontent = reader["content"].ToString();
                }
                reader.Close();
                providers.CloseConn();
            }
        }
    }
}

namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class commendlist : UserControl
    {
        protected DataGrid DataGrid1;
        public int i = 1;
        public system sys;

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
            string mySql = "select top 30 character_name, webcommendidnum,webcommendgift from (select propid as user_idx, b.webcommendidnum ,b.webcommendgift from mhcmember..chr_log_info a,(select top 30 webcommendid,count(webcommendid) as webcommendidnum,sum(webcommendgift) as webcommendgift from mhcmember..chr_log_info where webcommendid<>'' group by webcommendid order by webcommendgift desc,webcommendidnum desc) b where a.id_loginid=b.webcommendid)a,(select user_idx,character_name from mhgame..tb_character a where character_grade = (select max(character_grade) as  character_grade from (select distinct user_idx,character_grade from mhgame..tb_character) b where a.user_idx=user_idx) and substring(character_name,1,1)!='@' ) b where a.user_idx=b.user_idx order by webcommendgift desc,webcommendidnum desc";
            this.sys = new system();
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs(mySql, "DataGrid1");
                this.DataGrid1.DataBind();
            }
        }
    }
}
